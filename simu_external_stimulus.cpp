#include <sstream>
#include<vector>
#include<fstream>
#include<iostream>
#include<string>
#include<random>
#include<limits>

#include "new_models.h"

void input_gen(double H, std::vector<double>& input){

	std::random_device dev;
	std::mt19937 gen(dev());
	std::uniform_real_distribution<double> sampler(0, 1.0);
	for(std::vector<double>::size_type i = 0; i != input.size(); ++ i){
		if(sampler(gen) < 0.5){
			input[i] = 1.0*H;
		}
		else{
			input[i] = 0.0;
		}
	}
}

void input_gen2(double H, std::vector<double>& input){
	for(unsigned int i = 0; i != 250; ++i){
		input[i] = H*1.0;
	}
}

unsigned int unique_simu_key_gen(){
	std::random_device dev;
	std::mt19937 gen(dev());
	std::uniform_int_distribution<unsigned int> sampler(0, std::numeric_limits<unsigned int>::max());
	return sampler(gen);
}

// simu call .out N P fpatts gmma strength tau_fast eps H
int main(int argc, char* argv[]){

	std::cout << argc << std::endl;

	unsigned int N = std::stoi(argv[1]);
	unsigned int P = std::stoi(argv[2]);
	double fpatts = std::stod(argv[3]);
	double gamma = std::stod(argv[4]);
	double strength = std::stod(argv[5]);
	double tau_fast = std::stod(argv[6]);
	double eps = std::stod(argv[7]);
	double H = std::stod(argv[8]);

	unsigned int uid = unique_simu_key_gen();

	std::vector<double> input(N, 0.0);
	//input_gen(H, input);
	input_gen2(H, input);

	RateNet net(N, gamma, fpatts);

	for(unsigned int i = 0; i != P; ++ i){
		net.add_random_fpattern(strength);
	}

	net.random_state_gen(0.0, 1.0);
	net.make_hebb();
	net.start_fast_matrix();

	unsigned TIME = 50000;
	unsigned int no_input_time = 10000;

	std::stringstream projections_filename;
	std::stringstream netstate_filename;
	std::stringstream input_filename;
	std::stringstream patterns_filename;
	std::stringstream meta_filename;

	meta_filename << "data/newRate/random_stimulus/N500/" << "meta_key=" << uid << "_.txt";

	projections_filename << "data/newRate/random_stimulus/N500/" << "projections_key=" << uid << "_.txt";

	input_filename << "data/newRate/random_stimulus/N500/" << "input_key=" << uid << "_.txt";

	patterns_filename << "data/newRate/random_stimulus/N500/" << "patterns_key=" << uid << "_.txt";

	netstate_filename << "data/newRate/random_stimulus/N500/" << "netstate_key=" << uid << "_.txt";

	std::cout << projections_filename.str() << std::endl;

	std::ofstream projection_file(projections_filename.str());
	std::ofstream meta_file(meta_filename.str());
	std::ofstream input_file(input_filename.str());
	std::ofstream patterns_file(patterns_filename.str());
	std::ofstream netstate_file(netstate_filename.str());

	// writes the input to a file
	for(std::vector<double>::iterator it = input.begin(); it != input.end(); ++ it){
		input_file << *it << " ";
	}
	input_file << "\n";
	input_file.close(); // should I remove '\n' at the end of file?

	// write patterns to file
	for(unsigned int u = 0; u != P; ++ u){
		for(int& xiu : net.get_pattern(u)){
			patterns_file << xiu << " ";
		}
		patterns_file << "\n";
	}
	patterns_file.close();

	// write meta file
	meta_file << "N=" << N << "\n";
	meta_file << "P=" << P << "\n";
	meta_file << "fpatts=" << fpatts << "\n";
	meta_file << "gamma=" << gamma << "\n";
	meta_file << "str=" << strength << "\n";
	meta_file << "tau_fast=" << tau_fast << "\n";
	meta_file << "eps=" << eps << "\n";
	meta_file << "H=" << H << "\n";
	meta_file.close();

	net.set_fast_hebb_par(tau_fast, eps);
	std::vector<double>& state_ref = net.get_state();
	for(unsigned int t = 0; t != no_input_time; ++ t){

		for(std::vector<double>::size_type i = 0; i != N; ++i){
			netstate_file << state_ref[i] << " ";
		}
		netstate_file << "\n";

		for(unsigned int u = 0; u != P; ++ u){
			projection_file << net.projection(u) << " ";
		}
		projection_file << "\n";
		net.net_hcalc('x');
		net.net_state_update();
		net.w_fast_update();
	}
	for(unsigned int t = no_input_time; t != TIME; ++t){

		for(std::vector<double>::size_type i = 0; i != N; ++i){
			netstate_file << state_ref[i] << " ";
		}
		netstate_file << "\n";

		for(unsigned int u = 0; u != P; ++ u){
			projection_file << net.projection(u) << " ";
		}
		projection_file << "\n";
		net.net_hcalc('x');
		net.net_state_update(input);
		net.w_fast_update();
	}

	projection_file.close();
	netstate_file.close();

	std::cout << "Finished simu!" << std::endl;

	return 0;
}
