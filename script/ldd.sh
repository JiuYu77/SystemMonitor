#!/bin/bash
# bash ldd.sh app_name

lib_dir=$PWD"/lib"
Target=$1
lib_array=($(ldd $Target | grep -o "/.*" | grep -o "/.*/[^[:space:]]*"))
$(mkdir $lib_dir)
for Variable in ${lib_array[@]}
do
    cp "$Variable" $lib_dir
done