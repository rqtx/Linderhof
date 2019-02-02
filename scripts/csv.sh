#!/bin/bash

dir=("config1" "config2" "config3" "config4" "config5" "config6")
subDir=("GetSet" "Stat")

for i in "${dir[@]}"
do
  cd $i

  for j in "${subDir[@]}"
  do
    cd $j
    ~/Projetos/Linderhof/scripts/pcap2csv.py 
    ~/Projetos/Linderhof/scripts/LhfAnalyser.py
    cd ..
  done
  
  cd ..
done
