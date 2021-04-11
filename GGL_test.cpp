#include <fstream>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"models.h"


/*Call: ./name.out pattern_strength alpha*/
int main(int argc, char* argv[]){
	spiking::GGL_parameters model_par;
	spiking::GGL_synapses_parameters syn_par;

	std::string fname = "data_test/testing_GGL_model_NET_w=";
	fname = fname + argv[1] + "_N=100.txt";

	std::string pattern_fname = "data_test/pattern_GGL_model_NET_w=";
	pattern_fname = pattern_fname + argv[1] + "_N=100.txt";

	model_par.mu = 0.0;
	model_par.theta = 0.0;
	model_par.v_base = 0.0;
	model_par.v_reset = 0.0;
	model_par.Gamma = 1.0;
	model_par.N = 10;

	syn_par.eps_fast = 0.09;
	syn_par.eps_slow = 0.001;
	syn_par.tau = 600.0;

	spiking::GGL net(model_par, syn_par, std::stod(argv[2]));

	std::vector<double> pat(model_par.N, 1);

	net.add_exterior_pattern(pat, static_cast<double>(model_par.N));
	net.add_random_pattern(0.5, std::stod(argv[1]));
	net.add_pn_pattern(model_par.N/2, static_cast<double>(model_par.N)*3.0);
	net.make_hebb_matrix();

	int TIME = 10000;

	std::ofstream file_dynamics, file_pattern;
	file_dynamics.open(fname);
	file_pattern.open(pattern_fname);
	std::vector<int>& ref_to_state = net.get_state();
	for(double& elem : net.get_pattern(0)){
		file_pattern << elem << " ";
	}
	file_pattern << std::endl;
	file_pattern.close();
	//std::cout << "Hebbian matrix" << std::endl;
	//std::vector<std::vector<double>>& hebbmat = net.get_hebb();
	//for(std::vector<std::vector<double>>::size_type i = 0; i != model_par.N; ++i){
	//	for(std::vector<std::vector<double>>::size_type j = 0; j != model_par.N; ++j){
	//			std::cout << hebbmat[i][j] << " ";
	//	}
	//	std::cout << "\n";
	//}
	std::cout << std::endl;
	std::cout << "---- Dynamics! ---- \n\n" << std::endl;
	for(int t = 0; t != TIME; ++t){
		net.net_xtt();
		switch(net.rho()){
			case 0:
				net.random_spike();
				break;
			default:
				break;

		}
		for(int elem : ref_to_state){
			file_dynamics << elem << " ";
		}
		file_dynamics << "\n";
		net.net_activity();
		net.net_vtt();
	}
	file_dynamics.close();

	return 0;
}
