#!/bin/bash

qt="/home/jyu/install/Qt5.14.2/5.14.2/gcc_64/"
luptate=$qt/bin/lupdate
lrelease=$qt/bin/lrelease


# 生成 或 更新.ts files
generate_update_Ts()
{
    # 备份
    cp res/language/SystemMonitor_zh_CN.ts res/language/SystemMonitor_zh_CN.ts.bak
    cp res/language/SystemMonitor_en_US.ts res/language/SystemMonitor_en_US.ts.bak
    # 生成 或 更新
    $luptate src -ts res/language/SystemMonitor_zh_CN.ts res/language/SystemMonitor_en_US.ts
}
# 生成 或 更新.qm files
generate_update_Qm()
{
    $lrelease res/language/SystemMonitor_en_US.ts
    $lrelease res/language/SystemMonitor_zh_CN.ts
}

generate_update_Ts
generate_update_Qm
