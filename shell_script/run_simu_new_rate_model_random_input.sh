#!/bin/bash

N=500
vecP=(5 10 15 20)

gma=20
Fvec=(5 10 15 20)

fpatt=$(bc <<< "scale=3; 0.5")

tau=400
eps=$(bc <<< "scale=6; 0.004")

Hvec=(0.5 0.6 0.7 0.8 0.9 1.0 1.2 1.3 1.4 1.5)

let nproc=6
let k=0


for P in "${vecP[@]}";
do
	str=$(bc <<< "scale=3; $P*2.0")

	for H in "${Hvec[@]}";
	do
			if ((k>5)); then
				while ((k!=0))
				do
					k=`ps -el | grep simu_external | wc -l`
					echo $k
					sleep 1
				done
				k=0
			fi
			setsid nohup ./simu_external_stimulus.o $N $P $fpatt $gma $str $tau $eps $H > simu.log 2>&1 < simu.log &
			((++k))
	done
done
