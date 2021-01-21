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
/*
rate::ItineratedMap::ItineratedMap(int n_elements, float gma): N(n_elements), gamma(gma){
    std::random_device dev;
    eng.seed(dev());
    dist.param(std::uniform_real_distribution<float>::param_type(0.0, 1.0));
    h_vec = std::vector<float>(N, 0.0);

    P = 0;

}

rate::ItineratedMap::ItineratedMap(int n_elements, float a, float b, float gma): N(n_elements), gamma(gma){
    std::random_device dev;
    eng.seed(dev());
    dist.param(std::uniform_real_distribution<float>::param_type(a, b));

    for(std::vector<float>::size_type i = 0; i != N; ++i){
        State.push_back(dist(eng));
    }
    eng.seed(dev());
    dist.reset();

    h_vec = std::vector<float>(N, 0.0);
    P = 0;
}

void rate::ItineratedMap::anti_hebb_param(float _tau, float eps){
    tau = _tau;
    epsilon = eps;
}

void rate::ItineratedMap::model_param(float gma){
    gamma = gma;
}

void rate::ItineratedMap::store_random_state(float a, float b){
    //TODO: this snippet is used a lot throughout the code. Put it in a func
    std::random_device dev;
    dist.param(std::uniform_real_distribution<float>::param_type(a, b));
    eng.seed(dev());
    dist.reset();

    std::vector<float> xi;
    for(std::vector<float>::size_type i = 0; i != N; ++i){
        xi.push_back(dist(eng));
    }

    Patterns.insert(std::pair<int, std::vector<float>>(P, xi));
    ++P;

}

void rate::ItineratedMap::store_external_state(std::vector<float>& vec){
    Patterns.insert(std::pair<int, std::vector<float>>(P, vec)); //TODO: verify if deleting vec in main deletes the pattern!
    ++P;
}

void rate::ItineratedMap::store_pmone_random_state(float prob){
    std::random_device dev;
    dist.param(std::uniform_real_distribution<float>::param_type(0.0, 1.0));
    eng.seed(dev());
    dist.reset();

    std::vector<float> xi;
    for(std::vector<float>::size_type i = 0; i != N; ++i){
        if(dist(eng) < prob){
            xi.push_back(1.0);
        }
        else{
            xi.push_back(-1.0);
        }
    }
    Patterns.insert(std::pair<int, std::vector<float>>(P, xi));
    ++P;
}

std::vector<float>& rate::ItineratedMap::get_xi(int u){
    return Patterns[u];
}

std::vector<float>& rate::ItineratedMap::get_State(){
    return State;
}

std::vector<float>& rate::ItineratedMap::get_activity(){
    return h_vec;
}
std::vector<std::vector<float>>& rate::ItineratedMap::get_hebb(){
    return J_heb;
}

void rate::ItineratedMap::state_zero_random(){
    std::random_device dev;
    dist.param(std::uniform_real_distribution<float>::param_type(0.0, 1.0));
    eng.seed(dev());
    dist.reset();

    if(State.size() == 0){
        for(std::vector<float>::size_type i = 0; i != N; ++i){
            State.push_back(dist(eng));
        }
    }
    else if (State.size() == N){
        for(float &si : State){
            si = dist(eng);
        }
    }
}

void rate::ItineratedMap::state_zero_external(std::vector<float> &state0){
    if(State.size() == 0){
        for(float &s0i : state0){
            State.push_back(s0i);
        }
    }
    else if (State.size() == N){
        int aux = 0;
        for(float &si : State){
            si = state0[aux];
            ++aux;
        }
    }
}

void rate::ItineratedMap::make_JHebbian(){
    if(J_heb.size() == 0){
        for(std::vector<float>::size_type i = 0; i != N; ++i){
            std::vector<float> Jihebb;
            for(std::vector<float>::size_type j = 0; j != N; ++j){
                float aux = 0.0;
                for(int u = 0; u != P; ++u){
                    aux += Patterns[u][i]*Patterns[u][j];
                }
                if(i == j){
                    aux = 0.0;
                }
                Jihebb.push_back(aux/float(N)); //TODO: ask Osame if this normalization is correct. It seems off...
            }
            J_heb.push_back(Jihebb);
        }
    }
    else if(J_heb.size() == N){
        for(std::vector<float>::size_type i = 0; i != N; ++i){
            for(std::vector<float>::size_type j = 0; j != N; ++j){
                float aux = 0.0;
                for(int u = 0; u != P; ++u){
                    aux += Patterns[u][i]*Patterns[u][j];
                }
            J_heb[i][j] = aux/float(N); //TODO: ask Osame if this normalization is correct. It seems off...
            }
        }
    }
}

void rate::ItineratedMap::anti_Hebb_init(){
    if(J_nheb.size() == 0){
        for(std::vector<float>::size_type i = 0; i != N; ++i){
            std::vector<float> aux_i(N, 0);
            J_nheb.push_back(aux_i);
        }
    }
    else if(J_nheb.size() == N){
        for(std::vector<float>::size_type i = 0; i != N; ++i){
            for(std::vector<float>::size_type j = 0; j != N; ++j){
                J_nheb[i][j] = 0.0;
            }
        }
    }
}

void rate::ItineratedMap::anti_Hebb_update(){
    float aux1 = (1.0 - (1.0/tau));
    float aux2 = (epsilon/float(N));
    for(std::vector<float>::size_type i = 0; i != N; ++i){
        for(std::vector<float>::size_type j = 0; j != N; ++j){
            J_nheb[i][j] = (aux1*J_nheb[i][j]) - (aux2*(State[i]*State[j]));
        }
    }
}

void rate::ItineratedMap::activity_update(){
    for(std::vector<float>::size_type i = 0; i != N; ++i){
        float aux = 0.0;
        for(std::vector<float>::size_type j = 0; j != N; ++j){
            aux += (J_heb[i][j] + J_nheb[i][j])*State[j];
        }
        h_vec[i] = aux;
    }
}

void rate::ItineratedMap::state_update(){
    for(std::vector<float>::size_type i = 0; i != N; ++i){
        State[i] = std::tanh(gamma*h_vec[i]);
    }
}

void rate::ItineratedMap::state_update(std::vector<float>& input){
    for(std::vector<float>::size_type i = 0; i != N; ++i){
        State[i] = std::tanh(gamma*h_vec[i] + input[i]);
    }
}
*/
/* ------------ spiking models namespace ------------ */

void spiking::lif::hello(std::string msg){
    std::cout << msg << std::endl;
}