#include<iostream>
#include<fstream>
#include<string>
#include "new_models.h"

int main(int argc, char* argv[]){
	std::cout << "Hello" << std::endl;
	double gma = std::stod(argv[1]);
	double tau_fast = std::stod(argv[2]);
	double eps = std::stod(argv[3]);
	RateNet net(500, gma, 0.5);
	net.add_random_fpattern(1.0); //0
	net.add_random_fpattern(1.0); //1
	net.add_random_fpattern(1.0); //2
	net.add_random_fpattern(1.0); //3
	net.random_state_gen(0.0, 1);
	net.make_hebb();
	net.start_fast_matrix();

	std::ofstream out_file;
	out_file.open("new_rate_model_test.txt");
	char mode = 'x';
	net.set_fast_hebb_par(tau_fast, eps);
	for(int i = 0; i != 10000; ++i){
		for(unsigned int u = 0; u != net.get_P(); ++ u){
			out_file << net.projection(u) << " ";
		}
		out_file << "\n";
		net.net_hcalc(mode);
		net.w_fast_update();
		net.net_state_update();
	}
	out_file.close();
	return 0;
}
