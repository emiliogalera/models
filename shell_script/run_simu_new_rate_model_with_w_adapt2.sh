#! /bin/bash

tau=600
eps=$(bc <<< "scale=6; 0.009")

pnVec=(60)
#pnVec=(35 45 55 65 75)
N=500

vecSize=50


# change this to some other value







let nProcs=7
let k=1

for p in "${pnVec[@]}";
do
	declare -a gmaVec
	declare -a strVec
	str=$(bc <<< "scale=6; 0.2*$p")
	gma=2
	delta_gma=2
	delta_str=$(bc <<< "scale=6; (10*$p)/50")
	for i in $(seq 1 1 $vecSize);
	do
		strVec+=($str)
		gmaVec+=($gma)
		gma=$(($gma + $delta_gma))
		str=$(bc <<< "scale=6; $str + $delta_str")
	done

	mkdir data/newRate/best_m_with_adapt/N$N/P$p
	for gma in "${gmaVec[@]}";
	do
		    for str in "${strVec[@]}";
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
				setsid nohup ./simu_new_rate_model_with_w_adapt.o $N $p 0.5 $gma $str $tau $eps >simu.log 2>&1 < simu.log &
	    	done
	done
done
