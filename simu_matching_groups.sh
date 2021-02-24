#!/bin/bash

Hebb=(0 1)
percent_vec=(0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5)

tf="data/matching_groups"

for hb in "${Hebb[@]}"
do
    for x in "${percent_vec[@]}"
    do
        setsid nohup ./smg.out $x $tf $hb &
    done
done