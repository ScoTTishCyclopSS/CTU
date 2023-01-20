#!/bin/bash

clear
make || exit 1
for i in "01" "02" "03" "04" "05" "06" "07" "08" "09" "10"
do
    echo $i
    str="datapub/pub${i}.in"
    time ./hw6 <$str #>test.out
    echo "---------------"
done