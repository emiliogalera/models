#include<iostream>
#include<vector>
#include "models.h"

int main(){

    // create a network with 5 elements.
    rate::ItineratedMap bla2;
    bla2.Hello();

    /*bla2.state_zero_random(); //creates the initial state of the netw

    // adds a random state
    //bla2.store_external_state(vec0);
    bla2.store_random_state(-1.0, 1.0);
    bla2.store_random_state(-1.0, 1.0);
    bla2.anti_hebb_param(600.0, 0.009);
    //bla2.store_pmone_random_state(0.5);
    bla2.make_JHebbian(); // make the Hebbian matrix based on this single state
    std::cout << "This is the Hebbian connection matrix" << std::endl;
    for(std::vector<float> &vi : bla2.get_hebb()){
        for(float &si : vi){
            std::cout << si << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    bla2.anti_Hebb_init();
    for(float &si : bla2.get_State()){
            std::cout << si << " ";
        }
    std::cout << std::endl;
    for(float &si : bla2.get_activity()){
            std::cout << si << " ";
        }
    std::cout << std::endl;
    bla2.activity_update();
    for(float &si : bla2.get_activity()){
            std::cout << si << " ";
        }
    std::cout << std::endl;
    std::cout << "Stored state:" << std::endl;
    for(float &xi_i : bla2.get_xi(0)){
        std::cout << xi_i << " ";
    }
    std::cout << std::endl;

    std::cout << "Let's start the dynamics!\nThis first example does not have a anti-Hebb term" << std::endl;

    int TIME_ITER = 55;
    for(int t = 0; t != TIME_ITER; ++t){
        bla2.state_update();
        bla2.anti_Hebb_update();
        std::cout << "time step: State " << t << " --- ";
        for(float &si : bla2.get_State()){
            int ss;
            if(si < 0.0){
                ss = -1;
            }
            else if(si >= 0.0){
                ss = 1;
            }
            std::cout << ss << " ";
        }
        std::cout << std::endl;

        bla2.activity_update();
        std::cout << "time step: activity " << t << " --- ";
        for(float &si : bla2.get_activity()){
            std::cout << si << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;

    }

    std::cout << "corrent state: ";
    for(float &si : bla2.get_State()){
        int ss;
        if(si < 0.0){
            ss = -1;
        }
        else if(si >= 0.0){
            ss = 1;
        }
        std::cout << ss << " ";
    }
    std::cout << std::endl;

    std::cout << "stored state 0:  ";
    for(float &si : bla2.get_xi(0)){
        int ss;
        if(si < 0.0){
            ss = -1;
        }
        else if(si >= 0.0){
            ss = 1;
        }
        std::cout << ss << " ";
    }
    std::cout << std::endl;
    std::cout << "stored state 1:  ";
    for(float &si : bla2.get_xi(1)){
        int ss;
        if(si < 0.0){
            ss = -1;
        }
        else if(si >= 0.0){
            ss = 1;
        }
        std::cout << ss << " ";
    }
    std::cout << std::endl;*/
    return 0;
}