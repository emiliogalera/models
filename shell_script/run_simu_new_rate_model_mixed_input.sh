#!/bin/bash

N=1000
P=10

gma=200
F=50

fpatt=$(bc <<< "scale=3; 0.5")

tau=750
eps=$(bc <<< "scale=6; 0.0098")

Hvec=(0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 3.0)

let nproc=5
let k=0

for H in "${Hvec[@]}";
do
		if ((k==6)); then
			while ((k!=0))
			do
				k=`ps -el | grep simu_new_rate_m | wc -l`
				echo $k
				sleep 1
			done
			k=0
		fi
		setsid nohup ./simu_new_rate_model_mixed_inpup.out $N $P $fpatt $gma $F $tau $eps $H > simu.log 2>&1 < simu.log &
		((++k))
done
