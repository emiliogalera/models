#include<random>
#include<cmath>
#include "new_models.h"

void RateNet::random_setter(double a, double b){
	std::uniform_real_distribution<double>::param_type new_param(a, b);
	dis.param(new_param);
	dis.reset();
}

double RateNet::sample(){
	return dis(gen);
}

RateNet::RateNet(int net_size, double gma, double fval){
	N = net_size;
	gamma = gma;
	P = 0;
	f = fval; //TODO: add a way to make each pattern support its own f.
			  //this would mean to make f in to an array
	otau = 1.0;
	oeps = 0.0;
	for(std::vector<double>::size_type i = 0; i != N; ++i){
		state_vector.push_back(0.0);
		hvec.push_back(0.0);
	}
	std::random_device rd;
	gen = std::mt19937(rd());
}

void RateNet::add_random_pattern(double probability, double str){
	std::vector<int> pattern;
	unsigned int f = 0;
	random_setter(0.0, 1.0);

	for(std::vector<int>::size_type i = 0; i != N; ++i){
		if(sample() < probability){
			pattern.push_back(1);
			++f;
		}
		else{
			pattern.push_back(0);
		}
	}
	p_matrix.push_back(pattern);
	strvec.push_back(str);
	++ P;
}

void RateNet::add_random_10(unsigned int active_number, double str){
	std::vector<int> pattern(N, 0);
	unsigned int sum = 0;
	random_setter(0.0, 1.0);
	std::vector<int>::size_type position;
	while(sum != active_number){
		position = static_cast<std::vector<int>::size_type>(static_cast<double>(N)*sample());
		if(pattern[position] == 0){
			pattern[position] = 1;
			++ sum;
		}
	}
	p_matrix.push_back(pattern);
	strvec.push_back(str);
	++ P;
}

void RateNet::add_random_fpattern(double str){
	unsigned int Nf = static_cast<unsigned int>(static_cast<double>(N)*f);
	add_random_10(Nf, str);
}

void RateNet::random_state_gen(double a, double b){
	random_setter(a, b);
	for(std::vector<double>::size_type i = 0; i != N; ++ i){
		state_vector[i] = sample();
	}
}

void RateNet::make_hebb(){
	double aux;
	double xii, xij;
	for(std::vector<double>::size_type i = 0; i != N; ++ i){
		std::vector<double> line(N, 0.0);
		for(std::vector<double>::size_type j = 0; j != N; ++ j){
			if(i != j){
				aux = 0.0;
				for(std::vector<int>::size_type u = 0; u != P; ++ u){
					xii = static_cast<double>(p_matrix[u][i]) - f;
					xij = static_cast<double>(p_matrix[u][j]) - f;
					aux += strvec[u]*xii*xij;
				}
				line[j] = (1.0/(static_cast<double>(P*N)*(f*(1.0 - f))))*aux;
			}
		}
		w_matrix.push_back(line);
	}
}

void RateNet::start_fast_matrix(){
	for(std::vector<double>::size_type i = 0; i != N; ++ i){
		std::vector<double> line(N, 0.0);
		w_fast.push_back(line);
	}
}

double RateNet::hcalc_svar(std::vector<double>::size_type i){
	double aux = 0.0;
	for(std::vector<double>::size_type j = 0; j != N; ++ j){
		aux += w(i, j)*((2.0*state_vector[j]) - 1.0);
	}
	return aux;
}

double RateNet::hcalc_xvar(std::vector<double>::size_type i){
	double aux = 0.0;
	for(std::vector<double>::size_type j = 0; j != N; ++ j){
		aux += w(i, j)*state_vector[j];
	}
	return aux;
}

void RateNet::net_hcalc(char mode){
	if(mode == 's'){
		for(std::vector<double>::size_type i = 0; i != N; ++ i){
			hvec[i] = hcalc_svar(i);
		}
	}
	else if(mode == 'x'){
		for(std::vector<double>::size_type i = 0; i != N; ++ i){
			hvec[i] = hcalc_xvar(i);
		}
	}
}

double RateNet::state_update(std::vector<double>::size_type i, double input){
	double aux = gamma*(hvec[i] + input);
    return (0.5*(1.0 + (aux/(1 + std::abs(aux)))));
}
//double RateNet::state_update(std::vector<double>::size_type i, double input){
//	double aux = gamma*(hvec[i] + input);
//	return (0.5*(1.0 + std::tanh(aux)));
//}

void RateNet::net_state_update(){
	for(std::vector<double>::size_type i = 0; i != N; ++ i){
		state_vector[i] = state_update(i);
	}
}

void RateNet::net_state_update(std::vector<double>& input_vec){
	for(std::vector<double>::size_type i = 0; i != N; ++ i){
		state_vector[i] = state_update(i, input_vec[i]);
	}
}

void RateNet::net_state_update(std::vector<int>& input_vec){
	for(std::vector<double>::size_type i = 0; i != N; ++ i){
		state_vector[i] = state_update(i, static_cast<double>(input_vec[i]));
	}
}

void RateNet::set_fast_hebb_par(double tau, double eps){
	otau = 1.0 - (1.0/tau);
	oeps = eps/static_cast<double>(N);
}

void RateNet::w_fast_update(){
	//double aux1 = 1.0 - (1.0/tau);
	//double aux2 = eps/static_cast<double>(N);
	//TODO: this function updates the fast W matrix with S, check if it works
	//with X
	double xii, xij;
	for(std::vector<double>::size_type i = 0; i != N; ++ i){
		for(std::vector<double>::size_type j = 0; j != N; ++ j){
			if(i != j){
				//xii = (2.0*state_vector[i]) - 1.0;
				//xij = (2.0*state_vector[j]) - 1.0;
				xii = state_vector[i] - f;
				xij = state_vector[j] - f;
				w_fast[i][j] = (otau*w_fast[i][j]) - (oeps*((xii*xij)/(f*(1.0-f))));
			}
		}
	}
}

double RateNet::w(std::vector<double>::size_type i, std::vector<double>::size_type j){
	return w_matrix[i][j] + w_fast[i][j];
}

double RateNet::projection(std::vector<double>::size_type u){
	double aux = 0;
	double norm = 1.0/(static_cast<double>(N)*(f*(1.0 - f)));
	for(std::vector<double>::size_type i = 0; i != N; ++ i){
		aux += (static_cast<double>(p_matrix[u][i]) - f)*state_vector[i];
	}
	return norm*aux;
}

unsigned int RateNet::get_P(){
	return P;
}

std::vector<int>& RateNet::get_pattern(std::vector<int>::size_type u){
	return p_matrix[u];
}

std::vector<std::vector<double>>::size_type RateNet::get_sizeof_w_matrix(){
	std::vector<std::vector<double>>::size_type size = 0;
	for(std::vector<double>& line : w_matrix){
		size += sizeof(double)*line.capacity();
	}
	return size;
}

std::vector<std::vector<double>>::size_type RateNet::get_sizeof_w_matrix_fast(){
	std::vector<std::vector<double>>::size_type size = 0;
	for(std::vector<double>& line : w_fast){
		size += sizeof(double)*line.capacity();
	}
	return size;
}

std::vector<std::vector<int>>::size_type RateNet::get_sizeof_p_matrix(){
	std::vector<std::vector<int>>::size_type size = 0;
	for(std::vector<int>& line : p_matrix){
		size += sizeof(int)*line.capacity();
	}
	return size;
}
