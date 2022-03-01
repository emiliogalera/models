#include"new_models.h"

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cmath>

std::vector<int> tokenizer(std::string& string, std::string delim){
	std::size_t pos = 0;
	std::string token;
	std::vector<int> token_vector;
	while((pos = string.find(delim)) != std::string::npos){
		token = string.substr(0, pos);
		token_vector.push_back(std::stoi(token));
		string.erase(0, pos + delim.length());
	}
	return token_vector;
}

std::vector<double> tokenizer_double(std::string& string, std::string delim){
	std::size_t pos = 0;
	std::string token;
	std::vector<double> token_vector;
	while((pos = string.find(delim)) != std::string::npos){
		token = string.substr(0, pos);
		token_vector.push_back(std::stod(token));
		string.erase(0, pos + delim.length());
	}
	return token_vector;
}

double dist_calc(std::vector<double>& vec1, std::vector<double>& vec2){
	double dist = 0;
	for(std::vector<double>::size_type i = 0; i != vec1.size(); ++ i){
		dist += std::pow((vec1[i] - vec2[i]), 2.0);
	}
	return dist;
}

int main(){

	std::string line;
	std::ifstream file("data/newRate/chaos_test/patterns/P=10_N=500_f=0.5.txt");
	std::ifstream state1_file("data/newRate/chaos_test/state0/state_net1.txt");
	std::ifstream state2_file("data/newRate/chaos_test/state0/state_net2.txt");

	std::string delimeter(" ");
	std::vector<std::vector<int>> raw_patt_matrix;
	std::vector<double> state1, state2;

	if(file.is_open()){
		while(std::getline(file, line)){
			raw_patt_matrix.push_back(tokenizer(line, delimeter));
		}
		file.close();
	}
	else{
		std::cout << "unable to open file!" << std::endl;
	}

	if(state1_file.is_open()){
		while(std::getline(state1_file, line)){
			state1 = tokenizer_double(line, delimeter);
		}
		state1_file.close();
	}
	else{
		std::cout << "unable to load state1" << std::endl;
	}

	if(state2_file.is_open()){
		while(std::getline(state2_file, line)){
			state2 = tokenizer_double(line, delimeter);
		}
		state2_file.close();
	}
	else{
		std::cout << "unable to load state2" << std::endl;
	}

	RateNet net1(500, 20, 0.5);
	RateNet net2(500, 20, 0.5);
	for(std::vector<int>& patt : raw_patt_matrix){
		net1.add_exterior_pattern(patt, 10);
		net2.add_exterior_pattern(patt, 10);
	}

	net1.add_exterior_state(state1);
	net2.add_exterior_state(state2);

	net1.make_hebb();
	net2.make_hebb();

	net1.start_fast_matrix();
	net2.start_fast_matrix();

	unsigned int TIME = 100000;
	std::vector<double>& state1_ref = net1.get_state();
	std::vector<double>& state2_ref = net2.get_state();
	std::ofstream result_file("data/newRate/chaos_test/dist_eps=0.01_tau=400_TIME=100000.txt");
	net1.set_fast_hebb_par(400.0, 0.01);
	net2.set_fast_hebb_par(400.0, 0.01);

	for(unsigned int t = 0; t != TIME; ++ t){
		result_file << dist_calc(state1_ref, state2_ref) << "\n";
		net1.net_hcalc('x');
		net2.net_hcalc('x');
		net1.net_state_update();
		net2.net_state_update();
		net1.w_fast_update();
		net2.w_fast_update();
	}
	result_file.close();
	std::cout << "Finished simu!" << std::endl;
	return 0;
}
