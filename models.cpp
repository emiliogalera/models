#include<string>
#include<iostream>
#include<random>
#include<vector>
#include "models.h"

// The most simple constructor
rate::ItineratedMap::ItineratedMap(int n_elements): N(n_elements){
    std::random_device dev;
    eng.seed(dev());
    dist.param(std::uniform_real_distribution<float>::param_type(0.0, 1.0));

    P = 0;

}

// A constructor with a random initial state
// Use with caution, it sets the the state of each element in the interval to a, b!
rate::ItineratedMap::ItineratedMap(int n_elements, float a, float b): N(n_elements){
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

void rate::ItineratedMap::store_random_state(float prob){

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

// spiking models namespace
void spiking::lif::hello(std::string msg){
    std::cout << msg << std::endl;
}