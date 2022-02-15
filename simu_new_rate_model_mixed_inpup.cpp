#include<vector>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<random>
#include<cmath>
#include "new_models.h"


void input_gen(double H, std::vector<double>& input, std::vector<int>& mu_patterns, RateNet& nnet){
	//std::random_device dev;
	//std::mt19937 gen(dev());
	//std::uniform_int_distribution<unsigned int> input_sampler(0, P-1);
	unsigned int i = 0;
	for(int& mu : mu_patterns){
		i = 0;
		for(int& xi : nnet.get_pattern(mu)){
			input[i] = input[i] + static_cast<double>(xi);
			++i;
		}
	}
	double norm = H/(std::sqrt(static_cast<double>(mu_patterns.size())));
	for(unsigned int i = 0; i != input.size(); ++i){
		input[i] = norm*input[i];
	}
}

// simu call .out N P fpatts gmma strength tau_fast eps H
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
	double H = std::stod(argv[8]);


	std::vector<double> input(N, 0.0);
	std::vector<int> mu_input_patt{0, 1, 2, 3, 4, 5};


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
	input_gen(H, input, mu_input_patt, net);

	unsigned int TIME = 15000;
	unsigned int input_onset = 5000;

	std::stringstream filename;
	filename << "data/newRate/mix_patterns/explore/simu_N=" << N << "_gma="<< gamma << "_str=" << strength << "_P=" << P << "_f=" << fpatts << "_tau=" << tau_fast << "_eps=" << eps << "_H=" << H << "_.txt";

	std::cout << filename.str() << std::endl;

	std::ofstream out_file;
	out_file.open(filename.str());
	net.set_fast_hebb_par(tau_fast, eps);
	for(unsigned int t = 0; t != input_onset; ++ t){
		for(unsigned int u = 0; u != P; ++ u){
			out_file << net.projection(u) << " ";
		}
		out_file << "\n";
		net.net_hcalc('x');
		net.net_state_update();
		net.w_fast_update();
	}
	for(unsigned int t = input_onset; t != TIME; ++ t){
		for(unsigned int u = 0; u != P; ++ u){
			out_file << net.projection(u) << " ";
		}
		out_file << "\n";
		net.net_hcalc('x');
		net.net_state_update(input);
		net.w_fast_update();
	}

	out_file.close();

	std::cout << "Finished simu with P=" << P << ", tau=" << tau_fast << " and eps=" << eps <<std::endl;
	return 0;
}
