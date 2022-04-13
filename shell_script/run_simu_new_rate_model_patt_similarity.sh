#! /bin/bash

tau=500
eps=(0.002 0.004 0.006 0.008 0.01)

Pvec=(5)

P=10

N=1000

simVec=(0.05 0.1 0.15 0.20 0.25 0.3 0.35 0.4)

let k=1

for p in "${Pvec[@]}";
do
	for e in "${eps[@]}";
	do
		for s in "${simVec[@]}";
		do
			((++k))
			if ((k==7)); then
				while ((k!=0))
				do
					k=`ps -el | grep simu_patt_simil | wc -l`
					echo $k
					sleep 2
				done
				k=0
			fi
			setsid nohup ./simu_patt_similarity.o $N 8 5 2 0.5 20 15 $tau $e $s >simu.log 2>&1 < simu.log &

		done

	done

done
