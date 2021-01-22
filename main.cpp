#include<iostream>
#include<vector>
#include "models.h"

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

    rate::parameters simu_par;
    simu_par.epsilon = 0.009;
    simu_par.gamma = 10.0;
    simu_par.tau = 600.0;
    simu_par.N = 100;
    rate::ItineratedMap bla2(simu_par, -1.0, 1.0);

    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);
    bla2.add_random_pattern(0.5);

    bla2.make_hebb_matrix();

    //const std::vector<float>& s_vec_ref = bla2.get_State();
    //const std::vector<float>& h_vec_ref = bla2.get_activity();
    //const std::vector<float>& pattern_ref = bla2.get_xi(0);
    //const std::vector<std::vector<float>>& Hebb_ref = bla2.get_hebb();
    //const std::vector<std::vector<float>>& antiHebb_ref = bla2.get_antihebb();

    //std::cout << "Hebbian matrix:" << std::endl;
    //print_var(Hebb_ref);
    //std::cout << std::endl;

    //std::cout << "Printing State vector:" << std::endl;
    //print_var(s_vec_ref);
    //std::cout << std::endl;

    //std::cout << "Printing pattern:" << std::endl;
    //print_var(pattern_ref);
    //std::cout << std::endl;

    //std::cout << "Initiating activity vector h(t):" << std::endl;
    //print_var(h_vec_ref);
    //std::cout << std::endl;
    bla2.activity_update();

    //std::cout << "Activity updated:" << std::endl;
    //print_var(h_vec_ref);
    //std::cout << std::endl;

    int TIMER = 15000;
    //std::cout << "Initiating dynamics for" << TIMER << " time steps:" << std::endl;
    std::vector<float> m_vec;
    for(int t = 0; t != TIMER; ++t){
        bla2.antiHebb_update();
        bla2.state_update_tgh();
        bla2.activity_update();
        bla2.generate_m(m_vec);
        for(float& mu : m_vec){
            std::cout << mu << " ";
        }
        std::cout << std::endl;
    }

    //std::cout << "State :" << std::endl;
    //print_var(s_vec_ref);
    //std::cout << std::endl;

    return 0;
}