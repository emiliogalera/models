#include<string>
#include<iostream>
#include<random>
#include<vector>
#include "models.h"

// The most simple constructor
rate::ItineratedMap::ItineratedMap(int n_elements, float gma): N(n_elements), gamma(gma){
    std::random_device dev;
    eng.seed(dev());
    dist.param(std::uniform_real_distribution<float>::param_type(0.0, 1.0));
    h_vec = std::vector<float>(N, 0.0);

    P = 0;

}

// A constructor with a random initial state
// Use with caution, it sets the the state of each element in the interval to a, b!
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

void rate::ItineratedMap::store_random_state(){

    //TODO: this snippet is used a lot throughout the code. Put it in a func
    std::random_device dev;
    dist.param(std::uniform_real_distribution<float>::param_type(0.0, 1.0));
    eng.seed(dev());
    dist.reset();

    std::vector<float> xi;
    for(std::vector<float>::size_type i = 0; i != N; ++i){
        xi.push_back(dist(eng));
    }

    Patterns.insert(std::pair<int, std::vector<float>>(P, xi));
    ++P;

}

std::vector<float>& rate::ItineratedMap::get_xi(int u){
    return Patterns[u];
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

// spiking models namespace
void spiking::lif::hello(std::string msg){
    std::cout << msg << std::endl;
}