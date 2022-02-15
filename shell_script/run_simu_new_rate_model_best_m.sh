#! /bin/bash

declare -a gammaVec
declare -a strengthVec

pnVec=(35 45 55 65 75)
#pnVec=(5 15)
N=500

vecSize=50
delta=10

gma=0
str=0

for i in $(seq 1 1 $vecSize)
do
		echo $i
		gma=$(($gma + $delta))
		gammaVec+=($gma)
		str=$(($str + $delta))
		strengthVec+=($str)
done

let nProcs=7
let k=1

for p in "${pnVec[@]}";
do
	mkdir data/newRate/N$N/P$p
	for strength in "${strengthVec[@]}";
	do
		    for gamma in "${gammaVec[@]}";
	   		do
				#k=`ps -el | grep simu_new_rate_m | wc -l`
				((++k))
				setsid nohup ./simu_new_rate_model_best_m.o $N $p 0.5 $gamma $strength >simu.log 2>&1 < simu.log &
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
	    	done
	done
done
