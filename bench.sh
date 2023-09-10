#!/bin/bash
cd src
fList=$(ls -R -1)
echo $fList
#gprof -pmain.cpp -Q duelBot gmon.out