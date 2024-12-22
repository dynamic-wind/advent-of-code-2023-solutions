#!/usr/bin/env bash

file=$1
filename=$(basename $file)
filename_no_extension=${filename%.*}
script_path=$(dirname $0)
output=${script_path}/.bin/${filename_no_extension}

g++ $file -o $output -Wall -O3 -std=c++17 && $output
