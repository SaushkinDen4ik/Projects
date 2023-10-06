#!/bin/bash

if test -e data/data.txt; then
rm data/data.txt
fi

touch data/data.txt

if [ -z "$1" ]; then
    file='solution.cpp'
else
    file=$1
fi

mpic++ $file

for p in 1 2 3 4
do
    mpiexec -n $p ./a.out
    echo
done

rm pics/*
rm a.out

python3 graph.py
