#include<iostream>
#include<fstream>
#include<chrono>
#include<vector>
#include<string>
#include<numeric>
#include"../models.h"

int main(int argc, char* argv[]){

		int iter = 10;

		std::cout << argc << std::endl;

		std::ofstream file;

		file.open("output.txt");
		std::cout << file.is_open();

	// command line argumments
	// cast the argumments to use in the creation of the network
	std::vector<int>::size_type N = std::stoi(argv[1]);
	double mu = std::stoi(argv[2]);
	double gamma = std::stoi(argv[3]);
	double alpha = std::stoi(argv[4]);
	int P = std::stoi(argv[5]);

	// vector to store the time for each iteration
	double state_time = 0.0;
	double m_time = 0.0;
	double mp_time = 0.0;
	double v_time = 0.0;
	double write_time = 0.0;
	double activity_time = 0.0;

	spiking::SimpleGGL sggl(N, mu, gamma, alpha);

	std::vector<int>::size_type PN = std::stoi(argv[7]);
	std::cout << "Number of elements: "<< N << std::endl;
	std::cout << "Number of positive: "<< PN << std::endl;

	for(int p = 0; p != P; ++p){
		sggl.add_random_pm_pattern(PN, std::stod(argv[6]));
	}

	//time points
	auto time1 = std::chrono::steady_clock::now();
	auto time2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> deltat;
	//network dynamics
	unsigned int rho = 1;
	std::vector<double>& mref = sggl.get_mvec();

	for(int t = 0; t != iter; ++t){
		time1 = std::chrono::steady_clock::now();
		sggl.net_stt();
		time2 = std::chrono::steady_clock::now();
		//deltat = std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time2);
		deltat = time2 - time1;
		//std::cout << deltat.count();
		state_time += deltat.count();

		switch(rho){
			case 0:{
				sggl.random_spike();
				break;
			}
			default:{
				break;
			}
		}
		time1 = std::chrono::steady_clock::now();
		sggl.net_m_utt();
		time2 = std::chrono::steady_clock::now();
		deltat = time2 - time1;
		m_time += deltat.count();

		time1 = std::chrono::steady_clock::now();
		for(int i = 0; i != P; ++i){
			file << mref[i] << " ";
		}
		rho = sggl.get_rho();
		//file << rho;
		file << "\n";
		time2 = std::chrono::steady_clock::now();
		deltat = time2 - time1;
		write_time += deltat.count();

		time1 = std::chrono::steady_clock::now();
		//auto start = std::chrono::steady_clock::now();
		sggl.net_mp_utt();
		//auto end = std::chrono::steady_clock::now();
		time2 = std::chrono::steady_clock::now();
		//std::chrono::duration<double> elapsed_seconds = end-start;
		deltat = time2 - time1;
		//std::cout << elapsed_seconds.count() << std::endl;
		mp_time += deltat.count();

		time1 = std::chrono::steady_clock::now();
		sggl.net_activity();
		time2 = std::chrono::steady_clock::now();
		deltat = time2 - time1;
		activity_time += deltat.count();

		time1 = std::chrono::steady_clock::now();
		sggl.net_vtt();
		time2 = std::chrono::steady_clock::now();
		deltat = time2 - time1;
		v_time += deltat.count();
	}

	file.close();

	std::cout << "time elapsed for x[t] update: " << (state_time/static_cast<double>(iter)) << std::endl;
	std::cout << "time elapsed for m[t] update: " << m_time/static_cast<double>(iter) << std::endl;
	std::cout << "time elapsed for m'[t] update: " << mp_time/static_cast<double>(iter) << std::endl;
	std::cout << "time elapsed for act[t] update: " << activity_time/static_cast<double>(iter) << std::endl;
	std::cout << "time elapsed for v[t] update: " << v_time/static_cast<double>(iter) << std::endl;
	std::cout << "time elapsed for writing: " << write_time/static_cast<double>(iter) << std::endl;

	double total = (state_time + m_time + mp_time + v_time + write_time + activity_time)/static_cast<double>(iter);

	std::cout << "Total time elapsed for one iteration: " << total << std::endl;

return 0;
}
