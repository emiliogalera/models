#include <fstream>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"models.h"


int main(int argc, char* argv[]){
	spiking::GGL_parameters model_par;
	spiking::GGL_synapses_parameters syn_par;

	std::string fname = "data_test/testing_GGL_model_NET_w=";
	fname = fname + argv[1] + "_N=1000"+ ".txt";

	model_par.mu = 0.0;
	model_par.theta = 0.0;
	model_par.v_base = 0.0;
	model_par.v_reset = 0.0;
	model_par.Gamma = 1.0;
	model_par.N = 1000;

	syn_par.eps_fast = 0.09;
	syn_par.eps_slow = 0.001;
	syn_par.tau = 600.0;

	spiking::GGL net(model_par, syn_par);

	std::vector<double> pat(model_par.N, std::stod(argv[1]));

	net.add_exterior_pattern(pat, static_cast<double>(model_par.N));
	net.make_hebb_matrix();

	int TIME = 10000;
	int rho_int;
	std::ofstream file;
	file.open(fname);
	//TODO: add a function to revive de network if it dies!
	for(int t = 0; t != TIME; ++t){
		net.net_xtt();
		rho_int = net.rho();
		switch(rho_int){
			case 0:
				net.random_spike();
				break;
			default:
				break;

		}
		net.net_activity();
		net.net_vtt();
		file << rho_int << "\n";
	}
	file.close();

	return 0;
}
