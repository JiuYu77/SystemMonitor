#!/bin/bash
# bash ldd.sh app_name

lib_dir=$PWD"/lib"
Target=$1
lib_array=($(ldd $Target | grep -o "/.*" | grep -o "/.*/[^[:space:]]*"))

if [ -d "$lib_dir" ]; then
    echo "目录存在：$lib_dir"
else
    echo "目录不存在：$lib_dir"
    echo "创建目录：$lib_dir"
    mkdir $lib_dir
fi

for var in ${lib_array[@]}
do
    echo "copy  "$var"  to  "$lib_dir
    cp "$var" $lib_dir
done