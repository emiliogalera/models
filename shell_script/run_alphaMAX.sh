#!/bin/bash

let mu=0
let w0=0
let wmax=0
let pts=50
let k=0
let x=0
let F0=0
let Fmax=0
let F=0
let delta=0
let alpha0=0
let alphaM=0
let alpha=0
let delta_alpha=0
let p=0
alphapts=(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19)

Nvec=(1000 5000 10000)
mu=$(bc <<< "scale=3; 0.3")
w0=$(bc <<< "scale=6; 0.9")
wmax=$(bc <<< "scale=6; 3.2")


for N in "${Nvec[@]}";
do
	alpha0=$(bc <<< "scale=6; 6/$N")
	alphaM=$(bc <<< "scale=6; 0.15")
	delta_alpha=$(bc <<< "scale=6; ($alphaM - $alpha0)/20")
	alpha=$(bc <<< "scale=6; 6/$N")
	for ap in "${alphapts[@]}";
	do
		F0=$(bc <<< "scale=6; $w0*2.0*$p")
		Fmax=$(bc <<< "scale=6; $wmax*2.0*$p")
		delta=$(bc <<< "scale=6; ($Fmax - $F0)/$pts")

		p=$(bc <<< "($alpha*$N)/1")
		alpha=$(bc <<< "scale=6; $alpha + $delta_alpha")

		F=$F0
		echo $p
		for ((i=0 ; i != pts; ++i));
		do
			setsid nohup ./simu_SimpleGGL_fine.out $N $p $F 2.8 $mu &
			((++k))
			echo $F
			F=$(bc <<< "scale=6; $F + $delta")
			if ((k>10)); then
				x=$(ps -el | grep simu_Sim | wc -l)
				while((x!=0));
				do
					sleep 0.5
					x=$(ps -el | grep simu_Sim | wc -l)
				done
				k=0
			fi

		done
	done
done
