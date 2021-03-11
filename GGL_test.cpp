#include<iostream>
#include"models.h"


int main(){
	spiking::GGL_parameters model_par;
	spiking::GGL_synapses_parameters syn_par;

	model_par.mu = 0.0;
	model_par.theta = 1.0;
	model_par.v_base = 0.1;
	model_par.v_reset = -0.1;
	model_par.Gamma = 1.1;
	model_par.N = 100;

	syn_par.eps_fast = 0.09;
	syn_par.eps_slow = 0.001;
	syn_par.tau = 600.0;

	spiking::GGL net(model_par, syn_par);

	return 0;
}
