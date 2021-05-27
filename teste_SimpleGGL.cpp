#include"models.h"

int main(){
	
	spiking::SimpleGGL SGGLnet(10, 0.5, 1.1, 2.8);
	SGGLnet.add_random_pattern(0.5, 100.0);
	SGGLnet.add_random_pm_pattern(5, 111.0);
	SGGLnet.mp_utt(0);
	return 0;
}
