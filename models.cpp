#include<string>
#include<iostream>
#include<random>
#include "models.h"

// The most simple constructor
rate::ItineratedMap::ItineratedMap(int n_elements): N(n_elements){
    std::random_device dev;
    eng.seed(dev());
    dist.param(std::uniform_real_distribution<float>::param_type(0.0, 1.0));

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
};

void spiking::lif::hello(std::string msg){
    std::cout << msg << std::endl;
}