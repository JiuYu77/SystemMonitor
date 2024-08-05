#!/bin/bash

# 将编译好的程序，打包成tar.gz
name=`ls |grep ^SystemMonitor-[0-9]\.[0-9]\.[0-9]-linux-x86_64$`
echo "tar source_path: "$name


cp -r bin/ config/ lib/ res/ README.md SystemMonitor.desktop \
      SystemMonitor-*/

path=$name/script
if [ -d "$path" ]; then
    echo "目录存在：$path"
else
    echo "目录不存在：$path"
    echo "创建目录：$path"
    mkdir $path
fi
cp -r script/install.sh script/uninstall.sh \
      SystemMonitor-*/script/


tar_name=$name.tar.gz
echo $tar_name
tar -czf $tar_name SystemMonitor-*/
