#include<string>
#include<iostream>
#include<random>
#include<vector>
#include<cmath>
#include "models.h"

/* ------------ rate models namespace ------------ */
rate::ItineratedMap::ItineratedMap(rate::parameters& par, float a, float b){
    prepare_random_device(a, b);

    // IMPORTANT: in the constructor, epsilon and tau are already in the
    // dynamics form.
    net_par.N = par.N;
    net_par.gamma = par.gamma;
    net_par.epsilon = par.epsilon/float(par.N);
    net_par.tau = (1.0 - (1.0/par.tau));

    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        state_var.s_vec.push_back(draw());
        state_var.h_vec.push_back(0.0);
        std::vector<float> dummy(net_par.N, 0.0);
        state_var.Hebb_matrix.push_back(dummy);
        state_var.antiHebb_matrix.push_back(dummy);
    }
}

void rate::ItineratedMap::add_random_pattern(float prob){
    prepare_random_device(0.0, 1.0);
    std::vector<float> xi_u;
    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        if(draw() < prob){
            xi_u.push_back(1.0);
        }
        else{
            xi_u.push_back(-1.0);
        }
    }
    state_var.P_matrix.push_back(xi_u);
}

void rate::ItineratedMap::make_hebb_matrix(){
    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        for(std::vector<float>::size_type j = 0; j != net_par.N; ++j){
            float aux = 0.0;
            if(i != j){
                for(std::vector<std::vector<float>>::size_type u = 0; u != state_var.P_matrix.size(); ++u){
                    aux += state_var.P_matrix[u][i]*state_var.P_matrix[u][j];
                }
            }
            state_var.Hebb_matrix[i][j] = aux/float(net_par.N);
        }
    }
}

/*---- Probing methods ----*/
std::vector<float>& rate::ItineratedMap::get_xi(int u){
    return state_var.P_matrix[u];
}

std::vector<float>& rate::ItineratedMap::get_State(){
    return state_var.s_vec;
}

std::vector<float>& rate::ItineratedMap::get_activity(){
    return state_var.h_vec;
}
std::vector<std::vector<float>>& rate::ItineratedMap::get_hebb(){
    return state_var.Hebb_matrix;
}

std::vector<std::vector<float>>& rate::ItineratedMap::get_antihebb(){
    return state_var.antiHebb_matrix;
}

void rate::ItineratedMap::generate_m(std::vector<float>& m_recipient){
    if(!m_recipient.empty()){
        m_recipient.clear();
    }
    float mod_state = 0.0;
    for(std::vector<float>::size_type j = 0; j != net_par.N; ++j){
        mod_state += state_var.s_vec[j]*state_var.s_vec[j];
    }
    for(std::vector<float>::size_type u = 0; u != state_var.P_matrix.size(); ++u){
        float prod = 0.0;
        for(std::vector<float>::size_type j = 0; j != net_par.N; ++j){
            prod += state_var.s_vec[j]*state_var.P_matrix[u][j];
        }
        m_recipient.push_back(prod/(std::sqrt(mod_state)*std::sqrt(float(net_par.N))));
    }
}

/*---- Update methods ----*/
void rate::ItineratedMap::activity_update(){
    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        float aux = 0.0;
        for(std::vector<float>::size_type j = 0; j != net_par.N; ++j){
            aux += (state_var.Hebb_matrix[i][j] + state_var.antiHebb_matrix[i][j])*state_var.s_vec[j];
        }
        state_var.h_vec[i] = aux;
    }
}

void rate::ItineratedMap::state_update_tgh(){
    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        state_var.s_vec[i] = std::tanh(net_par.gamma*state_var.h_vec[i]);
    }
}

void rate::ItineratedMap::state_update_tgh(std::vector<float>& input){
    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        state_var.s_vec[i] = std::tanh(net_par.gamma*state_var.h_vec[i] + input[i]);
    }
}

void rate::ItineratedMap::state_update_rational(){
    float x;
    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        x = net_par.gamma*state_var.h_vec[i];
        state_var.s_vec[i] = x/(1.0 + std::abs(x));
    }
}

void rate::ItineratedMap::state_update_rational(std::vector<float>& input){
    float x;
    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        x = (net_par.gamma*state_var.h_vec[i]) + input[i];
        state_var.s_vec[i] = x/(1.0 + std::abs(x));
    }
}

void rate::ItineratedMap::antiHebb_update(){
    for(std::vector<float>::size_type i = 0; i != net_par.N; ++i){
        for(std::vector<float>::size_type j = 0; j != net_par.N; ++j){
            state_var.antiHebb_matrix[i][j] = (net_par.tau*state_var.antiHebb_matrix[i][j]) - (net_par.epsilon*(state_var.s_vec[i]*state_var.s_vec[j]));
        }
    }
}

/*---- Private methods ----*/
// Prepares the random number generator
void rate::ItineratedMap::prepare_random_device(float a, float b){
    std::random_device dev;
    device.eng.seed(dev());
    device.dist.param(std::uniform_real_distribution<float>::param_type(a, b));
    device.dist.reset();
}

// draws a random number from the prepared distribution.
// Always prepare the distribution before drawing from it.
float rate::ItineratedMap::draw(){
    return device.dist(device.eng);
}

/* ------------ spiking models namespace ------------ */

void spiking::lif::hello(std::string msg){
    std::cout << msg << std::endl;
}