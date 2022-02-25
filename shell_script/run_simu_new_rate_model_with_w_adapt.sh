#! /bin/bash

declare -a tauVec
declare -a epsVec
pnVec=(20)
#pnVec=(35 45 55 65 75)
N=500

vecSize=50
delta=20

# change this to some other value
gma=20
str=10

tau=10
eps=$(bc <<< "scale=6; 0.0001")
delta_eps=$(bc <<< "scale=6; 0.002")
for i in $(seq 1 1 $vecSize);
do
		tauVec+=($tau)
		epsVec+=($eps)
		tau=$(($tau + $delta))
		eps=$(bc <<< "$eps + $delta_eps")
done

let nProcs=7
let k=1

for p in "${pnVec[@]}";
do
	mkdir data/newRate/best_m_with_adapt/N$N/P$p
	for ttau in "${tauVec[@]}";
	do
		    for eeps in "${epsVec[@]}";
	   		do
				#k=`ps -el | grep simu_new_rate_m | wc -l`
				((++k))
				#sleep 1
				if ((k==7)); then
					while ((k!=0))
					do
						k=`ps -el | grep simu_new_rate_m | wc -l`
						echo $k
						sleep 2
					done
					k=1
				fi
				setsid nohup ./simu_new_rate_model_with_w_adapt.o $N $p 0.5 $gma $str $ttau $eeps >simu.log 2>&1 < simu.log &
	    	done
	done
done
