#ifndef SYS_H
#define SYS_H

#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>

class Sys
{
public:
    Sys();
    ~Sys();
    
    struct sysinfo info;  // sysinfo系统相关信息的结构体
    int hours;
    int minutes;
    int seconds;
    void get_sysinfo();
    long get_host_runtime();
    void sec_to_HMS(long sec, int *hours, int *minutes, int *seconds);
    void host_runtime_to_HMS();
};

#endif // SYS_H