#include<iostream>
#include<vector>
#include "models.h"


// Function used for printing.
void print_var(const std::vector<float>& vec){
    for(float si : vec){
            std::cout << si << " ";
    }
    std::cout << std::endl;
}

void print_var(const std::vector<std::vector<float>>& vec){
    for(std::vector<float> lin : vec){
        for(float& si : lin){
            std::cout << si << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(){

    // Create a parameter variable for simulation
    rate::parameters simu_par;
    simu_par.eps_ah = 0.02;
    simu_par.eps_h = 0.001;
    simu_par.gamma = 10.0;
    simu_par.tau = 100.0;
    simu_par.N = 100;

    // Instanciate a ItineratedMap object with an initial state
    // in the continuous range (-1, 1)
    rate::ItineratedMap bla2(simu_par, -1.0, 1.0);

    // Stores 10 random patterns
    bla2.add_random_pattern(0.5, 2.5);
    bla2.add_random_pattern(0.5, 2.5);
    bla2.add_random_pattern(0.5, 2.5);
    bla2.add_random_pattern(0.5, 2.5);
    bla2.add_random_pattern(0.5, 2.5);
    bla2.add_random_pattern(0.5, 2.5);
    //bla2.add_random_pattern(0.5, 1.0);
    //bla2.add_random_pattern(0.5, 1.0);
    //bla2.add_random_pattern(0.5, 1.0);
    //bla2.add_random_pattern(0.5, 1.0);

    // Make the hebbian matrix based on the stored patterns
    bla2.make_hebb_matrix();

    // Initial activity
    bla2.activity_update();

    // Number of iterations the simulation lasts
    int TIMER = 15000;

    // A vector to store the projection of the current network state on to
    // the various stored patterns
    std::vector<float> m_vec;

    // Simple simulation scheme
    for(int t = 0; t != TIMER; ++t){
        bla2.antiHebb_update();
        //bla2.hebb_update();
        //bla2.state_update_tgh();
        bla2.state_update_rational();
        bla2.activity_update();
        bla2.generate_m(m_vec);
        for(float& mu : m_vec){
            std::cout << mu << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}