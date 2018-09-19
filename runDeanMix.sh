#!/bin/bash

string=""
for i in input/deanYoung*.txt
do
    string="$i,$string"
done

./bin/runTxtMix.exe $string
