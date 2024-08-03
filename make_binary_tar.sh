#!/bin/bash

# 将编译好的程序，打包成tar.gz

cp -r bin/ config/ res/ init_desktop.sh README.md SystemMonitor.desktop \
      SystemMonitor-*/

name=`ls |grep ^SystemMonitor-[0-9]\.[0-9]\.[0-9]-linux-x86_64$`

echo $name
echo
tar -czf $name.tar.gz SystemMonitor-*/

