#include"models.h"

int main(){
	
	spiking::SimpleGGL SGGLnet(100, 0.5, 1.1, 2.8);
	SGGLnet.add_random_pattern(0.5, 100.0);
	SGGLnet.add_random_pm_pattern(100, 111.0);
	return 0;
}
