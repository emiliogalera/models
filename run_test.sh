#!/bin/bash

# fixed parameters
fs=5
fw=2.5
N=100
Hebb=0
prob=0.5
tf="data_test"

# script variables
nproc='ps -e -o psr= | sort | uniq | wc -l > nproc'
#echo $(nproc)


# array parameters
nsp=(0 1 2 3 4 5 6 7 8 9 10)
nwp=(10 9 8 7 6 5 4 3 2 1 0)

index_array=(0 1 2 3 4 5 6 7 8 9 10)

for idx in "${index_array[@]}"
do
#of_strong_patterns F_strong #of_weak_patterns F_weak Prob target_folder
    setsid nohup ./teste.out $N $Hebb ${nsp[idx]} $fs ${nwp[idx]} $fw $prob $tf  >test.log 2>&1 < test.log &
    #echo $N $Hebb ${nsp[idx]} $fs ${nwp[idx]} $fw $prob $tf
done