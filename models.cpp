#include <math.h>
#include<string>
#include<iostream>
#include<random>
#include<vector>
#include<cmath>
#include<numeric>
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
spiking::GGL::GGL(GGL_parameters ggl_par, GGL_synapses_parameters syn_par, double alpha){
	pars.mu = ggl_par.mu;
	pars.theta = ggl_par.theta;
	pars.v_base = ggl_par.v_base;
	pars.v_reset = ggl_par.v_reset;
	pars.Gamma = ggl_par.Gamma;
	pars.N = ggl_par.N;

	synp.eps_fast = (syn_par.eps_fast/(static_cast<double>(pars.N)));
	synp.eps_slow = (syn_par.eps_slow/(static_cast<double>(pars.N)));
	synp.tau = 1.0 - (1.0/syn_par.tau);

	prepare_random_device(0.0, 1.0);

	std::vector<double> dummy(pars.N, 0.0);
	for(std::vector<double>::size_type i = 0; i != pars.N; ++i){
		vecs.h_vec.push_back(0.0);
		vecs.x_vector.push_back(0);
		vecs.v_vector.push_back(alpha*draw());
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

double spiking::GGL::phy(std::vector<double>::size_type i){
	double aux = pars.Gamma*(vecs.v_vector[i] - pars.theta);
	return (aux < 0.0) ? 0.0 : aux/(1.0 + aux);
}

/*---- Supporting functionds ----*/
double spiking::GGL::activity(std::vector<std::vector<double>>::size_type i){
	double act = 0.0;
	for(std::vector<double>::size_type j = 0; j != pars.N; ++j){
		act += (mat.fast_matrix[i][j] + mat.slow_matrix[i][j])*vecs.x_vector[j];
	}
	// Should be N - 1?
	return act/static_cast<double>(pars.N);
}

void spiking::GGL::net_activity(){
	for(std::vector<double>::size_type i = 0; i != pars.N; ++i){
		vecs.h_vec[i] = activity(i);
	}
}

void spiking::GGL::add_exterior_pattern(std::vector<double>& pat, double strength){
	std::vector<double> aux;
	double si;
	for(double elem : pat){
		si = (2.0*elem) - 1.0;
		aux.push_back(si);
	}
	mat.pattern_matrix.push_back(aux);
	vecs.pattern_strength.push_back(strength);
}

void spiking::GGL::add_random_pattern(double prob, double strength){
	std::vector<double> pattern;
	prepare_random_device(0.0, 1.0);
	for(std::vector<double>::size_type i = 0; i != pars.N; ++i){
		if(draw() < prob){
			pattern.push_back(1.0);
		}
		else{
			pattern.push_back(-1.0);
		}
	}
	vecs.pattern_strength.push_back(strength);
	mat.pattern_matrix.push_back(pattern);
}

void spiking::GGL::add_pn_pattern(std::vector<double>::size_type pn, double str){
	std::vector<double> patt(pars.N, -1);
	std::vector<double>::size_type flag = 0;
	prepare_random_device(0.0, 1.0);
	std::vector<double>::size_type candidate;
	while(flag != pn){
		candidate = static_cast<std::vector<double>::size_type>(static_cast<double>(pars.N)*draw());
		if(patt[candidate] == -1.0){
			patt[candidate] = 1.0;
			++flag;
		}
	}
	mat.pattern_matrix.push_back(patt);
	vecs.pattern_strength.push_back(str);
}

void spiking::GGL::add_orthogonal_pair(double str){
	std::vector<double> p1(pars.N, -1);
	std::vector<double> p2;
	for(std::vector<double>::size_type i = 0; i != pars.N/2; ++i){
		p1[i] = 1.0;
	}
	// build p2 by rotating p1 for 1/4 of N to the right
	for(std::vector<double>::iterator i = p1.end() - (pars.N/4); i != p1.end(); ++i){
		p2.push_back(*i);
	}
	for(std::vector<double>::iterator i = p1.begin(); i != p1.end() - (pars.N/4); ++i){
		p2.push_back(*i);
	}
	mat.pattern_matrix.push_back(p1);
	vecs.pattern_strength.push_back(str);
	mat.pattern_matrix.push_back(p2);
	vecs.pattern_strength.push_back(str);

}

void spiking::GGL::random_spike(){
	double aux = static_cast<double>(pars.N - 1)*draw();
	vecs.x_vector[static_cast<std::vector<int>::size_type>(aux)] = 1;
}

/*---- Synapses functions----*/
void spiking::GGL::make_hebb_matrix(){
	double aux;
	for(std::vector<std::vector<double>>::size_type i = 0; i != pars.N; ++i){
		for(std::vector<double>::size_type j = 0; j != pars.N; ++j){
			aux = 0.0;
			if(i != j){
				for(std::vector<std::vector<double>>::size_type mu = 0; mu != mat.pattern_matrix.size(); ++mu){
					aux += mat.pattern_matrix[mu][i]*mat.pattern_matrix[mu][j]*vecs.pattern_strength[mu];
				}
			}
			mat.slow_matrix[i][j] = aux/(static_cast<double>(pars.N)*static_cast<double>(mat.pattern_matrix.size()));
		}
	}
}

void spiking::GGL::slow_syn_update(){
	double si, sj;
	for(std::vector<std::vector<double>>::size_type i = 0; i != pars.N; ++i){
		for(std::vector<double>::size_type j = 0; j != pars.N; ++j){
			si = static_cast<double>((2*vecs.x_vector[i]) - 1);
			sj = static_cast<double>((2*vecs.x_vector[j]) - 1);
			mat.slow_matrix[i][j] = (i == j) ? 0.0 : mat.slow_matrix[i][j] - (synp.eps_slow*si*sj);
		}
	}
}

void spiking::GGL::fast_syn_update(){
	double si, sj;
	for(std::vector<std::vector<double>>::size_type i = 0; i != pars.N; ++i){
		for(std::vector<double>::size_type j = 0; j != pars.N; ++j){
			si = static_cast<double>((2*vecs.x_vector[i]) - 1);
			sj = static_cast<double>((2*vecs.x_vector[j]) - 1);
			mat.fast_matrix[i][j] = (i == j) ? 0.0 : (synp.tau*mat.fast_matrix[i][j]) - (synp.eps_fast*si*sj);
		}
	}
}

/*---- Dynamics functions ----*/
double spiking::GGL::vtt(std::vector<double>::size_type i){
	double res = (vecs.x_vector[i] == 1) ? pars.v_reset : (pars.mu*(vecs.v_vector[i] - pars.v_base)) + pars.v_base + activity(i);
	return res;
}

int spiking::GGL::xtt(std::vector<int>::size_type i){
	int res = (vecs.x_vector[i] == 1) ? 0 : static_cast<int>(draw() < phy(i));
	return res;
}

void spiking::GGL::net_vtt(){
	for(std::vector<double>::size_type i = 0; i != pars.N; ++i){
		vecs.v_vector[i] = vtt(i);
	}
}

void spiking::GGL::net_xtt(){
	for(std::vector<int>::size_type i = 0; i != pars.N; ++i){
		vecs.x_vector[i] = xtt(i);
	}
}

/*---- Probing functions ----*/
int spiking::GGL::rho(){
	return std::accumulate(vecs.x_vector.begin(), vecs.x_vector.end(), 0);
}

std::vector<int>& spiking::GGL::get_state(){
	return vecs.x_vector;
}

std::vector<double>& spiking::GGL::get_vvec(){
	return vecs.v_vector;
}

//TODO: Add protection against mu out of range!
std::vector<double>& spiking::GGL::get_pattern(std::vector<std::vector<double>>::size_type mu){
	return mat.pattern_matrix[mu];
}

std::vector<std::vector<double>>& spiking::GGL::get_hebb(){
	return mat.slow_matrix;
}

std::vector<std::vector<double>>& spiking::GGL::get_antiHebb(){
	return mat.fast_matrix;
}


/* ---- SimpleGGL class methods ---- */
/*private methods*/
void spiking::SimpleGGL::prepare_random_device(double a, double b){
	std::random_device dev;
    device.eng.seed(dev());
    device.dist.param(std::uniform_real_distribution<double>::param_type(a, b));
    device.dist.reset();
}

double spiking::SimpleGGL::draw(){
	return device.dist(device.eng);
}

double spiking::SimpleGGL::phy(std::vector<double>::size_type i){
	double aux = gma*vstate[i];
	return aux/(1.0 + aux);
}

/*constructor*/
spiking::SimpleGGL::SimpleGGL(std::vector<int>::size_type Nelem, double mupar, double gamma, double alp){
	N = Nelem;
	mu = mupar;
	gma = gamma;
	Pnbr = 0;
	prepare_random_device(0.0, 1.0);
	rho = 0;

	for(std::vector<double>::size_type i = 0; i != Nelem; ++i){
		vstate.push_back(draw()*alp);
		sstate.push_back(-1);
	}

}

/*---- Supporting functions ----*/
void spiking::SimpleGGL::add_random_pattern(double prob, double strength){
	prepare_random_device(0.0, 1.0);
	std::vector<int> patt;
	double sum = 0.0;
	for(std::vector<int>::size_type i = 0; i != N; ++i){
		if(draw() < prob){
			patt.push_back(1);
			sum += 1.0;
		}
		else{
			patt.push_back(-1);
			sum += -1.0;
		}
	}
	patt_matrix.push_back(patt);
	patt_sum.push_back(sum/static_cast<double>(N));
	mvec_prime.push_back(0.0);
	vstrg.push_back(strength);
	++Pnbr;
}

void spiking::SimpleGGL::add_random_pm_pattern(std::vector<int>::size_type Pn, double strength){
	std::vector<int> patt(N, -1);
	std::vector<int>::size_type flag = 0;
	std::vector<int>::size_type candidate;
	double sum = -(static_cast<double>(N) - Pn) + Pn;
	while(flag != Pn){
		candidate = static_cast<std::vector<int>::size_type>(static_cast<double>(N)*draw());
		if(patt[candidate] == -1){
			patt[candidate] = 1;
			++flag;
		}
	}
	patt_matrix.push_back(patt);
	vstrg.push_back(strength);
	patt_sum.push_back(sum/static_cast<double>(N));
	mvec_prime.push_back(0.0);
	++Pnbr;
}

/*---- Dynamic functions----*/
double spiking::SimpleGGL::m_utt(std::vector<double>::size_type u){
	int proj = 0;
	for(std::vector<int>::size_type i = 0; i != N; ++i){
		proj += sstate[i]*patt_matrix[u][i];
	}
	return static_cast<double>(proj)/static_cast<double>(N);
}

double spiking::SimpleGGL::mp_utt(std::vector<double>::size_type u){
	return vstrg[u]*(m_utt(u) + patt_sum[u]);
}
double spiking::SimpleGGL::vtt(std::vector<double>::size_type i){
	return i;
}
/*-----------------------------------*/
