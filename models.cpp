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

    P = 0;

}

// A constructor with a random initial state
// Use with caution, it sets the the state of each element in the interval to a, b!
rate::ItineratedMap::ItineratedMap(int n_elements, float a, float b, float gma): N(n_elements), gamma(gma){
    std::random_device dev;
    eng.seed(dev());
    dist.param(std::uniform_real_distribution<float>::param_type(a, b));

    for(int i = 0; i != N; ++i){
        State.push_back(dist(eng));
    }
    eng.seed(dev());
    dist.reset();

    P = 0;
}

void rate::ItineratedMap::anti_hebb_param(float _tau, float eps){
    tau = _tau;
    epsilon = eps;
}

void rate::ItineratedMap::store_random_state(){

    //TODO: this snippet is used a lot throughout the code. Put it in a func
    std::random_device dev;
    dist.param(std::uniform_real_distribution<float>::param_type(0.0, 1.0));
    eng.seed(dev());
    dist.reset();

    std::vector<float> xi;
    for(int i = 0; i != N; ++i){
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
        for(int i = 0; i != N; ++i){
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

// spiking models namespace
void spiking::lif::hello(std::string msg){
    std::cout << msg << std::endl;
}