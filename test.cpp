#include<iostream>
#include<vector>
#include<string>
#include<fstream>
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

/*test call fingerprint:
 *./test.out N Hebb/noHebb(0, 1) #of_strong_patterns F_strong #of_weak_patterns F_weak Prob target_folder
 */
int main(int argc, char* argv[]){

    //convert commandline arguments
    int N = std::stoi(argv[1]);
    int boolHebb = std::stoi(argv[2]);
    int nsp = std::stoi(argv[3]);
    float fstrong = std::stof(argv[4]);
    int nwp = std::stoi(argv[5]);
    float fweak = std::stof(argv[6]);
    float prob = std::stof(argv[7]);
    std::string target = argv[8];

    // Create a parameter variable for simulation
    rate::parameters simu_par;
    simu_par.eps_ah = 0.02; //0.02
    simu_par.eps_h = 0.001;
    simu_par.gamma = 10.0;
    simu_par.tau = 100.0; //100
    simu_par.N = N; //might cause a warning!

    // Instanciate a ItineratedMap object with an initial state
    // in the continuous range (-1, 1)
    rate::ItineratedMap rnn_model(simu_par, -1.0, 1.0);

    // Stores P random patterns
    //first stores the strong patterns
    for(int i = 0; i != nsp; ++i){
        rnn_model.add_random_pattern(prob, fstrong);
    }
    for(int i = 0; i != nwp; ++i){
        rnn_model.add_random_pattern(prob, fweak);
    }
    // Make the hebbian matrix based on the stored patterns
    rnn_model.make_hebb_matrix();

    // Initial activity
    rnn_model.activity_update();

    // Number of iterations the simulation lasts
    int TIMER = 15000;

    // A vector to store the projection of the current network state on to
    // the various stored patterns
    std::vector<float> m_vec;

    // builds the path for the target file
    target = target + "/" + "simu_N=" + argv[1] + "_Hebb=" + argv[2] + "_nsp=" + argv[3] + "_FS=" + argv[4] + "_nwp=" + argv[5] + "_FW=" + argv[6] + ".txt";

    std::ofstream outoput_fs;
    outoput_fs.open(target); //opens a file stream

    //std::string line; // string to build the line to file

    // Simple simulation scheme
    for(int t = 0; t != TIMER; ++t){
        rnn_model.antiHebb_update();
        if(boolHebb){
            rnn_model.hebb_update();
        }
        //rnn_model.state_update_tgh();
        rnn_model.state_update_rational();
        rnn_model.activity_update();
        rnn_model.generate_m(m_vec);
        for(float& mu : m_vec){
            //line += std::to_string(mu) + " ";
            outoput_fs << mu << " ";
        }
        outoput_fs << "\n";
    }
    outoput_fs.close();

    return 0;
}