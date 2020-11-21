#include<iostream>
#include "models.h"
int main(){

    rate::ItineratedMap bla(100);
    rate::ItineratedMap bla2(100, 0.1, 0.5);
    spiking::lif bla_lif;

    bla_lif.hello("Hello from lif");

    return 0;
}

