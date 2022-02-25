#include<vector>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<random>
#include "new_models.h"


// simu call .out N P fpatts gmma strength tau_fast eps
int main(int argc, char* argv[]){
	// command line variables
	std::cout << argc << std::endl;

	unsigned int N = std::stoi(argv[1]);
	unsigned int P = std::stoi(argv[2]);
	double fpatts = std::stod(argv[3]);
	double gamma = std::stod(argv[4]);
	double strength = std::stod(argv[5]);
	double tau_fast = std::stod(argv[6]);
	double eps = std::stod(argv[7]);

	//std::random_device dev;
	//std::mt19937 gen(dev());
	//std::uniform_real_distribution<double> input_sampler(0.0, 1.0);
	//std::vector<double> input(N, 0.0);


	// create network
	RateNet net(N, gamma, fpatts);

	// add patterns
	for(unsigned int i = 0; i != P; ++ i){
		net.add_random_fpattern(strength);
	}

	// generate initial state
	net.random_state_gen(0.0, 1.0);

	// generate hebb matrix
	net.make_hebb();

	// start anti-hebb matrix
	net.start_fast_matrix();

	unsigned int TIME = 7500;

	std::stringstream filename;
	filename << "data/newRate/best_m_with_adapt/N" << N << "/" << "P" << P << "/" << "simu_" << "P=" << P<< "_f=" << fpatts << "_tau=" << tau_fast << "_eps=" << eps << "_.txt";

	std::cout << filename.str() << std::endl;

	std::ofstream out_file;
	out_file.open(filename.str());
	net.set_fast_hebb_par(tau_fast, eps);
	for(unsigned int t = 0; t != TIME; ++ t){
		for(unsigned int u = 0; u != P; ++ u){
			out_file << net.projection(u) << " ";
		}
		out_file << "\n";
		net.net_hcalc('x');
		net.net_state_update();
		net.w_fast_update();
	}

	out_file.close();

	std::cout << "Finished simu with P=" << P << ", tau=" << tau_fast << " and eps=" << eps <<std::endl;
	return 0;
}
