#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "models.h"

//signature call name.out X% folder_path Hebb_dinamic_flag
int main(int argc, char* argv[]){

    // Simulation parameters
    rate::parameters simu_par;
    simu_par.eps_ah = 0.009; //0.009
    simu_par.eps_h = 0.0;
    simu_par.gamma = 10.0;
    simu_par.tau = 600.0; //600
    simu_par.N = 100; //might cause a warning!

    // Create the network with parameters simu_par and initial state between -1, 1
    rate::ItineratedMap net(simu_par, -1.0, 1.0);

    // Basic pattern used to create the similarity groups
    std::vector<double> pattern(simu_par.N, 0.0);

    /* Creates 3 sub-groups, each with 3 patterns. Patterns belonging to the same
      group share X% similarity.*/

    double percent = std::stof(argv[1]);
    int n_percent = static_cast<int>(static_cast<double>(simu_par.N)*percent);

    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Number of groups and number of patterns per group
    int NGROUP = 3;
    int PATTERN_PER_GROUP = 3;
    double PATTERN_STRENGHT = 9.0;

    // loop the 3 groups
    for(int group = 0; group != NGROUP; ++group){
        // create the 'signature' of the group
        for(int i = 0; i != n_percent; ++i){
            if(dist(gen) < 0.5){
                pattern[i] = 1.0;
            }
            else{
                pattern[i] = -1.0;
            }
        }
        // creates the 3 pattern for each group
        for(int pat = 0; pat != PATTERN_PER_GROUP; ++pat){
            // changes the N-n_percent elements of the vector
            for(std::vector<double>::size_type j = n_percent; j != simu_par.N; ++j){
                if(dist(gen) < 0.5){
                    pattern[j] = 1.0;
                }
                else{
                    pattern[j] = -1.0;
                }
            }
            net.add_exterior_pattern(pattern, PATTERN_STRENGHT);
        }
        gen.seed(dev());
        dist.reset();
    }

    int TIME = 15000;
    std::string file_name = argv[2];
    file_name = file_name + "/" + "nGroup=" + std::to_string(NGROUP) + "_patternsPerGroup=" + std::to_string(PATTERN_PER_GROUP) + "_strenght=" + std::to_string(PATTERN_STRENGHT) + "_similarity=" + std::to_string(n_percent) + "_hebb=" + argv[3] + ".txt";

    std::ofstream output_fs;
    output_fs.open(file_name);

    // Make the hebbian matrix based on the stored patterns
    net.make_hebb_matrix();

    // Initial activity
    net.activity_update();

    // A vector to store the projection of the current network state on to
    // the various stored patterns
    std::vector<double> m_vec;
    for(int t = 0; t != TIME; ++t){
        net.antiHebb_update();
        if(std::stoi(argv[3])){
            net.hebb_update();
        }
        net.state_update_tgh();
        //net.state_update_rational();
        net.activity_update();
        net.generate_m(m_vec);
        for(double& mu : m_vec){
            //line += std::to_string(mu) + " ";
            output_fs << mu << " ";
        }
        output_fs << "\n";

    }
    output_fs.close();


    return 0;
}
