#!/bin/bash

wvec=(0.0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5 0.55 0.6 0.65 0.7 0.75 0.8 0.85 0.9 0.95 1.0 1.05 1.1 1.15 1.2 1.25 1.3 1.35 1.4 1.45 1.5 1.55 1.6 1.65 1.7 1.75 1.8 1.85 1.9 1.95 2.0 2.05 2.1 2.15 2.2 2.25 2.3 2.35 2.4 2.45)
echo "Hello"
let k=0
let x=0
for w in "${wvec[@]}";
do
	setsid nohup ./teste_SimpleGGL.out $w >test.log 2>&1 < test.log &
	((++k))
	echo $w
	if ((k==6)); then
		x=$(ps -el | grep *teste_SimpleGGL* | wc -l)
		while ((x!=0));
		do
			sleep 1
			x=$(ps -el | grep teste_SimpleGGL | wc -l)
		done
		k=0
	fi
done
