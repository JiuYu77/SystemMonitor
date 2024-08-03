#include "monitor/cpu_mem.h"


CpuMemory::CpuMemory()
{}
CpuMemory::~CpuMemory()
{}
/**
 * @description: 获取某个时刻的CPU负载
 * @param {CPU_OCCUPY} *cpu
 * @author: Ming
 */
void CpuMemory::get_cpuoccupy(CPU_OCCUPY *cpu)
{
    char buff[BUFFSIZE];
    FILE *fd;

    /* /proc/stat 包含了系统启动以来的许多关于kernel和系统的统计信息，
    其中包括CPU运行情况、中断统计、启动时间、上下文切换次数、
    运行中的进程等等信息 */
    fd = fopen("/proc/stat", "r");
    if (fd == NULL) {
        perror("open /proc/stat failed\n");
        exit(0);
    }
    fgets(buff, sizeof(buff), fd);

    sscanf(buff, "%s %u %u %u %u", cpu->name, &cpu->user, &cpu->nice, &cpu->system, &cpu->idle);
    // sscanf(buff, "%s %u %u %u %u %u %u %u %u %u",
    //         cpu->name, &cpu->user, &cpu->nice, &cpu->system, &cpu->idle,
    //         &cpu->iowait, &cpu->irq, &cpu->softirq, &cpu->stealstolen, &cpu->guest, &cpu->guest_nice);
    fclose(fd);
}
/**
 * @description: 计算CPU占用率
 * @param {CPU_OCCUPY} *c1
 * @param {CPU_OCCUPY} *c2
 * @return {double}
 * @author: Ming
 */
double CpuMemory::cal_occupy(CPU_OCCUPY *c1, CPU_OCCUPY *c2)
{
    double t1, t2;
    double id, sd;
    double cpu_used;

    t1 = (double)(c1->user + c1->nice + c1->system + c1->idle);
    t2 = (double)(c2->user + c2->nice + c2->system + c2->idle);

    id = (double)(c2->user - c1->user);
    sd = (double)(c2->system - c1->system);

    cpu_used = (100 * (id + sd) / (t2 - t1));
    return cpu_used;
}
/**
 * @description: 获取cpu占用率
 * @param {double} *cpu_used
 * @author: Ming
 */
void CpuMemory::get_cpu_usage(double *cpu_used)
{
    CPU_OCCUPY cpu_0, cpu_1;

    get_cpuoccupy(&cpu_0);
    sleep(1);  // 实现循环监控CPU状态，间隔时间1s
    get_cpuoccupy(&cpu_1);

    *cpu_used = cal_occupy(&cpu_0, &cpu_1);
#if DEBUG
    printf("cpu: %.2f%\n", *cpu_used);
#endif
}
/**
 * @description: 获取内存占用率
 * @param {double} *mem_used
 */
void CpuMemory::get_mem_usage(double *mem_used)
{
    FILE *fd;
    /************
    ** /proc / meminfo中包含系统允许内存信息
    **
    ****/

    fd = fopen("/proc/meminfo", "r");
    if (fd == NULL) {
        perror("open /proc/meminfo failed\n");
        exit(0);
    }

    size_t bytes_read;
    size_t read;
    char *line = NULL;
    /************
     *mem_used = total-memavailable
     *******/
    int index = 0;
    int avimem = 0;

    while ((read = getline(&line, &bytes_read, fd)) != -1) {
        if (++index <= 2) {
            continue;
        }
        if (strstr(line, "MemAvailable") != NULL) {
            sscanf(line, "%*s%d%*s", &avimem);
	        break;
        }
    }
    free(line);
    int t = info.totalram / 1024.0;
    *mem_used = (t - avimem) * 100.0 / t;

#if DEBUG
    printf("\n");
    printf("total memory: %.3f\n", info.totalram * 1.0 / 1024 / 1024 / 1024);
    printf("free memory: %.3f\n", info.freeram * 1.0 / 1024 / 1024 / 1024);
    printf("shared memory: %.3f\n", info.sharedram * 1.0 / 1024 / 1024 / 1024);
    printf("buffer memory: %.3f\n", info.bufferram * 1.0 / 1024 / 1024 / 1024);
    printf("totalswap memory: %.3f\n", info.totalswap * 1.0 / 1024 / 1024 / 1024);
    printf("freeswap memory: %.3f\n", info.freeswap * 1.0 / 1024 / 1024 / 1024);
// printf("memory usage: %.2f%\n", *mem_used);
#endif
    fclose(fd);
}
/**
 * @description: CPU、内存、运行时间监控线程
 */
void CpuMemory::thread_core()
{
    // printf("%s\n", __FUNCTION__);
    get_sysinfo();

    // host_runtime_to_HMS();
    // printf("[run]:%6d hours\t%2d minutes\t%2d seconds\n", hours, minutes, seconds);

    get_cpu_usage(&cpu_used);
    // printf("[cpu_used]:%6.2f%%\n", cpu_used);

    get_mem_usage(&mem_used);
    // printf("[mem_used]:%6.2f%%\n", mem_used);
}