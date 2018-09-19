#!/bin/bash

string=""
for i in input/*.txt
do
    string="$i,$string"
done

./bin/runTxtMix.exe $string
