#include <fstream>
#include<vector>
#include<iostream>
#include<sstream>
#include<string>
#include<math.h>
#include"models.h"

//call signature .out
int main(int argc, char* argv[]){

	std::vector<int> Nvec{500, 1000, 5000, 10000, 50000};

	int Pmax = 50;

	std::stringstream pattern_name;
	std::ofstream pattern_file;

	for(int& N : Nvec){
		spiking::SimpleGGL sggl(N, 0.0, 1.0, 2.8);
		for(int p = 2; p <= Pmax; ++p){
			pattern_name << "data/N" << N << "/" << "P=" << p << "_.txt";
			pattern_file.open(pattern_name.str());
			for(int pp = 0; pp != p; ++pp){
				sggl.add_random_pm_pattern(N/2, 1);
			}
			for(int pp = 0; pp != p; ++pp){
				std::cout << "processing N = " << N << " network with P=" << p << " index of pattern: " << pp << std::endl;
				std::vector<int> patt = sggl.get_pattern(pp);
				for(int& s : patt){
					pattern_file << s << " ";
				}
				pattern_file << "\n";
			}
			pattern_file.close();
			pattern_name.str(std::string());

		}
	}
	return 0;
}
