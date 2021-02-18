#!/bin/bash

# fixed parameters
N=100
Hebb=0
prob=0.5
tf="data_test"
nsp=10
nwp=0

# simu=1 5 nsp and 5nwp

# simu2 10nsp, exploring the relation between patterns with different strength
# simu_id=2

# simu2 10nsp, grouping patterns by strength
simu_id=3

# script variables
nproc='ps -e -o psr= | sort | uniq | wc -l > nproc'
#echo $(nproc)


# array parameters
index_array=(0 1 2 3)
fw=(1 1 1 1)
fs=(0 1 2 3)

#let k=0
for idx in "${index_array[@]}"
do
#of_strong_patterns F_strong #of_weak_patterns F_weak Prob target_folder
    setsid nohup ./teste.out $N $Hebb $nsp ${fs[idx]} $nwp ${fw[idx]} $prob $tf $simu_id  >test.log 2>&1 < test.log &
#    ((++k))
#    if ((k==nproc)); then
#        echo "sleeping"
#        sleep 10
#    fi
    echo $N $Hebb $nsp ${fs[idx]} $nwp ${fw[idx]} $prob $tf $simu_id
done