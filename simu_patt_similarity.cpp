#include "new_models.h"

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cmath>
#include<random>


int make_signature(std::vector<int>& vec, int Nsig, int N){
	//random tools
	std::random_device dev;
	std::mt19937 gen(dev());
	std::uniform_real_distribution<double> sampler(0.0, 1.0);
	int positives = 0;
	for(int i = 0; i != N; ++i){
		vec[i] = 0;
	}
	for(int i = 0; i != Nsig; ++i){
		if(sampler(gen) < 0.5){
			vec[i] = 1;
			++positives;
		}
	}
	return positives;
}

void partial_random_patt(std::vector<int>& vec, int Nsig, int N, int group_pos, double fpt){
	int all_N_positive = static_cast<int>(fpt*static_cast<double>(N));
	int reminders = all_N_positive - group_pos;
	std::random_device dev;
	std::mt19937 gen(dev());
	std::uniform_int_distribution<int> sampler_int(Nsig, N-1);

	for(int i = Nsig; i != N; ++i){
		vec[i] = 0;
	}

	int sum = 0;
	int index = -1;
	while(sum != reminders){
		index = sampler_int(gen);
		if(vec[index] == 0){
			vec[index] = 1;
			++sum;
		}
	}

}

// program call: .out N PA PB PC fpatts gma strenght tau_f eps similarity
int main(int argc, char* argv[]){

	if(argc != 11){
		std::cout << "Wrong number of arguments. \n"
				<< "should be: N PA PB PC f Gma str tau eps sim" << std::endl;
		return 1;
	}

	// get command line arguments
	int N = std::stoi(argv[1]);
	int PA = std::stoi(argv[2]);
	int PB = std::stoi(argv[3]);
	int PC = std::stoi(argv[4]);
	double fpatts = std::stod(argv[5]);
	double gma = std::stod(argv[6]);
	double strength = std::stod(argv[7]);
	double tau_fast = std::stod(argv[8]);
	double eps = std::stod(argv[9]);
	double simil = std::stod(argv[10]);

	// dummy patterns to be used as a base to generate pattern signature and
	// group patterns
	std::vector<int> dummy_patt(N, 0);

	// Number of elements that are signature elements: always 0 - N_sig-1
	int N_sig = static_cast<int>(simil*static_cast<double>(N));

	// track the ones in the signature to keep the fpatts requirement
	int group_pos = make_signature(dummy_patt, N_sig, N);

	// instantiate the network
	RateNet net(N, gma, fpatts);

	// add patterns from group A
	for(int p = 0; p != PA; ++p){
		partial_random_patt(dummy_patt, N_sig, N, group_pos, fpatts);
		net.add_exterior_pattern(dummy_patt, strength);
	}

	// add patterns from group B
	group_pos = make_signature(dummy_patt, N_sig, N);
	for(int p = 0; p != PB; ++p){
		partial_random_patt(dummy_patt, N_sig, N, group_pos, fpatts);
		net.add_exterior_pattern(dummy_patt, strength);
	}

	// add patterns from group C
	group_pos = make_signature(dummy_patt, N_sig, N);
	for(int p = 0; p != PC; ++p){
		partial_random_patt(dummy_patt, N_sig, N, group_pos, fpatts);
		net.add_exterior_pattern(dummy_patt, strength);
	}

	net.random_state_gen(0.0, 1.0);
	net.make_hebb();
	net.start_fast_matrix();
	net.set_fast_hebb_par(tau_fast, eps);

	std::stringstream filename;
	filename  << "data/newRate/groups/N1000/P15/simu_PA=" << PA << "_PB=" << PB << "_PC=" << PC << "_tau=" << tau_fast << "_eps=" << eps << "_sim=" << simil << "_.txt";

	//prepare simulation
	unsigned int duration = 10000;
	std::ofstream out_file(filename.str());

	for(unsigned int t = 0; t != duration; ++t){
		for(unsigned int u = 0; u != static_cast<unsigned int>(PA + PB + PC); ++u){
			out_file << net.projection(u) << " ";
		}
		out_file << "\n";
		net.net_hcalc('x');
		net.net_state_update();
		net.w_fast_update();
	}
	out_file.close();

	return 0;
}
