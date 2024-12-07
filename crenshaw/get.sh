#!/bin/bash
#download all parts

for i in $(seq 1 16); do
    file="tutor"$i".txt"
    wget "http://compilers.iecc.com/crenshaw/"$file

done;