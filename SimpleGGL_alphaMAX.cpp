#include<iostream>
#include<string>
#include<vector>
#include"models.h"


int main(int argc, char* argv[]){

	std::cout << argc << std::endl;
	// network creation parameters
	std::vector<int>::size_type N = std::stoi(argv[1]);
	double gma = std::stod(argv[2]);
	double mu = std::stod(argv[3]);
	double net_alpha = std::stod(argv[4]);

	// number of patterns and strenght
	int P = std::stoi(argv[5]);
	double F = std::stod(argv[6]);
	std::vector<int>::size_type PN = N/2;

	spiking::SimpleGGL sggl(N, mu, gma, net_alpha);


	// Stores P patterns with strength F
	for(int p = 0; p != P; ++p){
		sggl.add_random_pm_pattern(PN, F);
	}

	//TODO:
	// - create 2 file pointes, one to store r[t] and another to store m[t]
	// - name files so each result goes to the right place
	// - store simulation data
	//Dynamics
	int time = 50000;
	unsigned int rho = 1;
	for(int t = 0; t != time; ++t){
		sggl.net_stt(); //update S[t]

		switch(rho){
			case 0:{
				sggl.random_spike();
				break;
			}
			default:{
				break;
			}
		}
		sggl.net_m_utt(); //update m[t]
		sggl.net_mp_utt(); //update m'[t]

		//activity update
		sggl.zero_activity();
		sggl.new_net_activity();
		sggl.norm_activity();
		sggl.net_vtt();
		rho = sggl.get_rho();

	}
	return 0;
}
