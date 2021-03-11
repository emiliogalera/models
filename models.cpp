#include<string>
#include<iostream>
#include<random>
#include<vector>
#include<cmath>
#include "models.h"

/* ------------ rate models namespace ------------ */
rate::ItineratedMap::ItineratedMap(rate::parameters& par, double a, double b){
    prepare_random_device(a, b);

    // IMPORTANT: in the constructor, epsilon and tau are already in the
    // dynamics form.
    net_par.N = par.N;
    net_par.gamma = par.gamma;
    net_par.eps_ah = par.eps_ah/double(par.N);
    net_par.eps_h = par.eps_h/double(par.N);
    net_par.tau = (1.0 - (1.0/par.tau));

    std::vector<double> dummy(net_par.N, 0.0);
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        state_var.s_vec.push_back(draw());
        state_var.h_vec.push_back(0.0);
        state_var.Hebb_matrix.push_back(dummy);
        state_var.antiHebb_matrix.push_back(dummy);
    }
}

rate::ItineratedMap::ItineratedMap(parameters& par, std::vector<double>& vec){
    net_par.N = par.N;
    net_par.gamma = par.gamma;
    net_par.eps_ah = par.eps_ah/double(par.N);
    net_par.eps_h = par.eps_h/double(par.N);
    net_par.tau = (1.0 - (1.0/par.tau));

    std::vector<double> dummy(net_par.N, 0.0);
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        state_var.s_vec.push_back(vec[i]);
        state_var.h_vec.push_back(0.0);
        state_var.Hebb_matrix.push_back(dummy);
        state_var.antiHebb_matrix.push_back(dummy);
    }
}

void rate::ItineratedMap::add_random_pattern(double prob, double strength){
    prepare_random_device(0.0, 1.0);
    std::vector<double> xi_u;
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        if(draw() < prob){
            xi_u.push_back(1.0);
        }
        else{
            xi_u.push_back(-1.0);
        }
    }
    state_var.P_matrix.push_back(xi_u);
    state_var.f_vec.push_back(strength);
}

void rate::ItineratedMap::add_exterior_pattern(std::vector<double>& vec, double strength){
    std::vector<double> xi_u(vec.begin(), vec.end());
    state_var.P_matrix.push_back(xi_u);
    state_var.f_vec.push_back(strength);
}

void rate::ItineratedMap::make_hebb_matrix(){
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        for(std::vector<double>::size_type j = 0; j != net_par.N; ++j){
            double aux = 0.0;
            if(i != j){
                for(std::vector<std::vector<double>>::size_type u = 0; u != state_var.P_matrix.size(); ++u){
                    aux += state_var.f_vec[u]*state_var.P_matrix[u][i]*state_var.P_matrix[u][j];
                }
            }
            //TODO: Ask Osame if the normalization is ok.
            state_var.Hebb_matrix[i][j] = aux/(double(net_par.N)*double(state_var.P_matrix.size()));
            //state_var.Hebb_matrix[i][j] = aux/(double(net_par.N));
        }
    }
}

/*---- Probing methods ----*/
std::vector<double>& rate::ItineratedMap::get_xi(int u){
    return state_var.P_matrix[u];
}

std::vector<double>& rate::ItineratedMap::get_State(){
    return state_var.s_vec;
}

std::vector<double>& rate::ItineratedMap::get_activity(){
    return state_var.h_vec;
}
std::vector<std::vector<double>>& rate::ItineratedMap::get_hebb(){
    return state_var.Hebb_matrix;
}

std::vector<std::vector<double>>& rate::ItineratedMap::get_antihebb(){
    return state_var.antiHebb_matrix;
}

void rate::ItineratedMap::generate_m(std::vector<double>& m_recipient){
    if(!m_recipient.empty()){
        m_recipient.clear();
    }
    double mod_state = 0.0;
    for(std::vector<double>::size_type j = 0; j != net_par.N; ++j){
        mod_state += state_var.s_vec[j]*state_var.s_vec[j];
    }
    for(std::vector<double>::size_type u = 0; u != state_var.P_matrix.size(); ++u){
        double prod = 0.0;
        for(std::vector<double>::size_type j = 0; j != net_par.N; ++j){
            prod += state_var.s_vec[j]*state_var.P_matrix[u][j];
        }
        m_recipient.push_back(prod/(std::sqrt(mod_state)*std::sqrt(double(net_par.N))));
    }
}

/*---- Update methods ----*/
void rate::ItineratedMap::activity_update(){
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        double aux = 0.0;
        for(std::vector<double>::size_type j = 0; j != net_par.N; ++j){
            aux += (state_var.Hebb_matrix[i][j] + state_var.antiHebb_matrix[i][j])*state_var.s_vec[j];
        }
        state_var.h_vec[i] = aux;
    }
}

void rate::ItineratedMap::state_update_tgh(){
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        state_var.s_vec[i] = std::tanh(net_par.gamma*state_var.h_vec[i]);
    }
}

void rate::ItineratedMap::state_update_tgh(std::vector<double>& input){
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        state_var.s_vec[i] = std::tanh(net_par.gamma*state_var.h_vec[i] + input[i]);
    }
}

void rate::ItineratedMap::state_update_rational(){
    double x;
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        x = net_par.gamma*state_var.h_vec[i];
        state_var.s_vec[i] = x/(1.0 + std::abs(x));
    }
}

void rate::ItineratedMap::state_update_rational(std::vector<double>& input){
    double x;
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        x = (net_par.gamma*state_var.h_vec[i]) + input[i];
        state_var.s_vec[i] = x/(1.0 + std::abs(x));
    }
}

void rate::ItineratedMap::antiHebb_update(){
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        for(std::vector<double>::size_type j = 0; j != net_par.N; ++j){
            //state_var.antiHebb_matrix[i][j] = (net_par.tau*state_var.antiHebb_matrix[i][j]) - (net_par.eps_ah*(state_var.s_vec[i]*state_var.s_vec[j]));
            state_var.antiHebb_matrix[i][j] = (i == j) ? 0.0 : ((net_par.tau*state_var.antiHebb_matrix[i][j]) - (net_par.eps_ah*(state_var.s_vec[i]*state_var.s_vec[j])));
        }
    }
}

void rate::ItineratedMap::hebb_update(){
    for(std::vector<double>::size_type i = 0; i != net_par.N; ++i){
        for(std::vector<double>::size_type j = 0; j != net_par.N; ++j){
            //state_var.Hebb_matrix[i][j] = state_var.Hebb_matrix[i][j] - (net_par.eps_h*(state_var.s_vec[i]*state_var.s_vec[j]));
            state_var.Hebb_matrix[i][j] = (i == j) ? 0.0 : (state_var.Hebb_matrix[i][j] - (net_par.eps_h*state_var.s_vec[i]*state_var.s_vec[j]));
        }
    }
}

/*---- Private methods ----*/
// Prepares the random number generator
void rate::ItineratedMap::prepare_random_device(double a, double b){
    std::random_device dev;
    device.eng.seed(dev());
    device.dist.param(std::uniform_real_distribution<double>::param_type(a, b));
    device.dist.reset();
}

// draws a random number from the prepared distribution.
// Always prepare the distribution before drawing from it.
double rate::ItineratedMap::draw(){
    return device.dist(device.eng);
}

/* ------------ spiking models namespace ------------ */
spiking::GGL::GGL(GGL_parameters ggl_par, GGL_synapses_parameters syn_par){
	pars.mu = ggl_par.mu;
	pars.theta = ggl_par.theta;
	pars.v_base = ggl_par.v_base;
	pars.v_reset = ggl_par.v_reset;
	pars.Gamma = ggl_par.Gamma;
	pars.N = ggl_par.N;

	synp.eps_fast = (syn_par.eps_fast/(static_cast<double>(pars.N)));
	synp.eps_slow = (syn_par.eps_slow/(static_cast<double>(pars.N)));
	synp.tau = syn_par.tau;

	prepare_random_device(0.0, 1.0);
	
	std::vector<double> dummy(pars.N, 0.0);
	for(std::vector<double>::size_type i = 0; i != pars.N; ++i){
		vecs.x_vector.push_back(0);
		vecs.v_vector.push_back(draw());
		mat.fast_matrix.push_back(dummy);
		mat.slow_matrix.push_back(dummy);
	}
}

/*---- private methods ----*/
void spiking::GGL::prepare_random_device(double a, double b){
	std::random_device dev;
    device.eng.seed(dev());
    device.dist.param(std::uniform_real_distribution<double>::param_type(a, b));
    device.dist.reset();
}

double spiking::GGL::draw(){
	return device.dist(device.eng);
}
