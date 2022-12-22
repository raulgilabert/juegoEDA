#!/bin/bash

make all

for i in {0..10}
do
    echo $i
    ./Game Dummy Dummy Dummy Nyaa -s $i < default.cnf > $i.res 2> default.err
    cat default.err | grep "Dummy got top score"
done
