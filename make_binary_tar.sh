#!/bin/bash

# 将编译好的程序，打包成tar.gz

cp -r bin/ config/ lib/ res/ install.sh uninstall.sh README.md SystemMonitor.desktop \
      SystemMonitor-*/

name=`ls |grep ^SystemMonitor-[0-9]\.[0-9]\.[0-9]-linux-x86_64$`

echo $name
tar_name=$name.tar.gz
echo $tar_name
tar -czf $tar_name SystemMonitor-*/

