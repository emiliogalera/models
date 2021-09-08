#include<string>
#include<iostream>
#include<fstream>
#include"../models.h"

// call name.out w
int main(int argc, char *argv[]){
	int TIME = 100000;
	unsigned int rho = 1;
	if(argc != 2){
		return 1;
	}

	std::ofstream output_file;
	std::string name = "data/teste/simple_ggl/teste_w=";
	output_file.open(name + argv[1] + ".txt");
	spiking::SimpleGGL SGGLnet(1000, 0.0, 1.0, 2.8);
	//SGGLnet.add_random_pattern(0.5, 100.0);
	SGGLnet.add_random_pm_pattern(1000, std::stod(argv[1]));
	for(int t = 0; t != TIME; ++t){
		SGGLnet.net_stt();	
		switch(rho){
			case 0:{
				SGGLnet.random_spike();
				//std::cout << "sdlkfj" << std::endl;
				break;
			}
			default:{
				break;
			}
		}
		SGGLnet.net_m_utt();
		SGGLnet.net_mp_utt();
		SGGLnet.net_vtt();
		rho = SGGLnet.rho_tt();
		output_file << rho << "\n";	
	}
	output_file << std::endl;
	output_file.close();
	return 0;
}
