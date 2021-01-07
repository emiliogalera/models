#include<iostream>
#include<vector>
#include "models.h"

int main(){

    rate::ItineratedMap bla(3, 3.0);
    rate::ItineratedMap bla2(3, 0.1, 0.5, 3.0);
    spiking::lif bla_lif;

    bla.state_zero_random();

    std::vector<float> svec0{1, 0.5, 0};
    bla2.state_zero_external(svec0);

    bla.store_random_state();
    bla.store_random_state();
    bla.store_random_state();
    bla.model_param(0.3);

    bla2.store_random_state();
    bla2.make_JHebbian();

    for(float &xi_i : bla.get_xi(0)){
        std::cout << xi_i << " ";
    }
    std::cout << std::endl;

    bla_lif.hello("Hello from lif");

    return 0;
}

