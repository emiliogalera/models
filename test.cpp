#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include "models.h"


// Function used for printing.
void print_var(const std::vector<double>& vec){
    for(double si : vec){
            std::cout << si << " ";
    }
    std::cout << std::endl;
}

void print_var(const std::vector<std::vector<double>>& vec){
    for(std::vector<double> lin : vec){
        for(double& si : lin){
            std::cout << si << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/*test call fingerprint:
 *./test.out N Hebb/noHebb(0, 1) #of_strong_patterns F_strong #of_weak_patterns F_weak Prob target_folder simu_id
 */
int main(int argc, char* argv[]){

    //convert commandline arguments
    int N = std::stoi(argv[1]);
    int boolHebb = std::stoi(argv[2]);
    int nsp = std::stoi(argv[3]);
    //double fstrong = std::stof(argv[4]);
    int fstrong = std::stof(argv[4]); // now fstrong selects a combination of
                                      // strong f to study how itinerancy
                                      // occours when the strengh of strong
                                      // patterns are not homogeneous
    int nwp = std::stoi(argv[5]);
    double fweak = std::stof(argv[6]);
    double prob = std::stof(argv[7]);
    std::string target = argv[8];

    // Create a parameter variable for simulation
    rate::parameters simu_par;
    simu_par.eps_ah = 0.009; //0.009
    simu_par.eps_h = 0.001;
    simu_par.gamma = 10.0;
    simu_par.tau = 600.0; //600
    simu_par.N = N; //might cause a warning!

    // Instanciate a ItineratedMap object with an initial state
    // in the continuous range (-1, 1)
    rate::ItineratedMap rnn_model(simu_par, -1.0, 1.0);

    // Stores P random patterns
    //first stores the strong patterns
    std::vector<std::vector<double>> fs_matrix;
    std::vector<double> fs_vec0{10.0, 10.0, 10.0, 6.0, 6.0, 6.0, 3.0, 3.0, 3.0, 3.0}; //0
    fs_matrix.push_back(fs_vec0);
    std::vector<double> fs_vec1{5.0, 5.0, 5.0, 4.0, 4.0, 4.0, 2.0, 2.0, 2.0, 1.0}; //1
    fs_matrix.push_back(fs_vec1);
    std::vector<double> fs_vec2{10.0, 10.0, 8.0, 8.0, 6.0, 6.0, 4.0, 4.0, 2.0, 2.0}; //2
    fs_matrix.push_back(fs_vec2);
    std::vector<double> fs_vec3{10.0, 10.0, 8.0, 8.0, 8.0, 6.0, 6.0, 6.0, 5.0, 5.0}; //3
    fs_matrix.push_back(fs_vec3);
    std::vector<double> fs_vec4{10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0}; //4
    fs_matrix.push_back(fs_vec4);

    for(int i = 0; i != nsp; ++i){
        rnn_model.add_random_pattern(prob, fs_matrix[fstrong][i]);
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
    std::vector<double> m_vec;

    // builds the path for the target file
    target = target + "/" + "simu_id=" + argv[9] + "_N=" + argv[1] + "_Hebb=" + argv[2] + "_nsp=" + argv[3] + "_FS=" + argv[4] + "_nwp=" + argv[5] + "_FW=" + argv[6] + ".txt";

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
        for(double& mu : m_vec){
            //line += std::to_string(mu) + " ";
            outoput_fs << mu << " ";
        }
        outoput_fs << "\n";
    }
    outoput_fs.close();

    return 0;
}
