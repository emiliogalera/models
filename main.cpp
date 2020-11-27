#include<iostream>
#include "models.h"
int main(){

    rate::ItineratedMap bla(100);
    rate::ItineratedMap bla2(100, 0.1, 0.5);
    spiking::lif bla_lif;

    bla.store_random_state();
    bla.store_random_state();
    bla.store_random_state();
    bla2.store_random_state();

    for(float &xi_i : bla.get_xi(0)){
        std::cout << xi_i << " ";
    }
    std::cout << std::endl;

    bla_lif.hello("Hello from lif");

    return 0;
}

