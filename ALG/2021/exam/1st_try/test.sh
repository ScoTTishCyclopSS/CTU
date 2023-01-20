#!/bin/bash

clear
make || exit 1

for i in "01" "02" "03" "04" "05" "06" "07" "08" "09" "10"
do
    echo "${i} test"
    echo
    str="datapub/pub${i}.in"
    time ./exam <$str
    echo "---------------"
done