#include "monitor/sys.h"

Sys::Sys()
{}
Sys::~Sys()
{}
/**
 * @description: 打开sysinfo文件，读取系统信息，如内存和运行时间信息
 */
void Sys::get_sysinfo()
{
    int ret = sysinfo(&this->info);
    if (ret != 0) {
        perror("get system information failed\n"); // #include <stdio.h>
        exit(0);  // #include <stdlib.h>
    }
}
/**
 * @description: 获取系统运行时间，单位：秒
 */
long Sys::get_host_runtime()
{
    get_sysinfo();
    return info.uptime;
}
/**
 * @description: 将秒转换为时分秒
 */
void Sys::sec_to_HMS(long sec, int *hours, int *minutes, int *seconds)
{
    *hours = sec / 3600;
    *minutes = (sec % 3600) / 60;
    *seconds = (sec % 3600) % 60;
}
/**
 * @description: 将系统运行时间转换为时分秒
 */
void Sys::host_runtime_to_HMS()
{
    sec_to_HMS(this->info.uptime, &hours, &minutes, &seconds);
}
