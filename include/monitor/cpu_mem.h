#ifndef CPU_MEM_H
#define CPU_MEM_H

#include <unistd.h>
#include <string.h>

#include "sys.h"


#define BUFFSIZE 512

#define DEBUG 0


typedef struct _CPU_PACKED {
    char name[16];
    unsigned int user;   // 用户模式
    unsigned int nice;   // 低优先级的用户模式
    unsigned int system; // 内核模式
    unsigned int idle;   // 空闲处理器时间
    unsigned int iowait;
    unsigned int irq;
    unsigned int softirq;
    unsigned int stealstolen;
    unsigned int guest;
    unsigned int guest_nice;
} CPU_OCCUPY;


class CpuMemory : Sys
{
public:
    CpuMemory();
    ~CpuMemory();

    double cpu_used;
    double mem_used;
    void get_cpuoccupy(CPU_OCCUPY *cpu);
    double cal_occupy(CPU_OCCUPY *c1, CPU_OCCUPY *c2);
    void get_cpu_usage(double *cpu_used);
    void get_mem_usage(double *mem_used);
    void thread_core();
};
#endif // CPU_MEM_H
