#ifndef NETSPEED_H
#define NETSPEED_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//网络监控部分
#include <arpa/inet.h>
#include <net/if.h>
// #include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string>

#include "utils/data.h"

#define WAIT_SECOND 1
#define MB 1024

#define DEBUG 0

/*总网速结构体*/
typedef struct _NET_SPEED {
    double d_speed; /*下行速度*/
    double u_speed; /*上行速度*/
    std::string d_speed_str; /*下行速度*/
    std::string u_speed_str; /*上行速度*/
} NET_SPEED;
/*收发数据包结构体*/
typedef struct _RTX_BYTES {
    long int tx_bytes;
    long int rx_bytes;
    struct timeval rtx_time;
} RTX_BYTES;
/*网卡设备信息结构体*/
typedef struct _NET_INTERFACE {
    char name[16];  /*网络接口名称*/
    char ip[16];    /*网口IP*/
    double d_speed; /*下行速度*/
    double u_speed; /*上行速度*/
    bool bool_down;  // 下行速度
    bool bool_up;  // 上行速度
    char mac[13];   /*网口MAC地址*/
    /*上下行速度级别 bit 7~0
     *bit[0]=d_speed
     *bit[1]=u_speed
     *1:MB/s 0:KB/s
     */
    unsigned char speed_level; /**/
    RTX_BYTES rtx0_cnt;
    RTX_BYTES rtx1_cnt;
    struct _NET_INTERFACE *next; /*链表指针*/
} NET_INTERFACE;


class NetSpeed
{
public:
    NetSpeed();
    ~NetSpeed();
    FILE *net_dev_file = NULL; // 文件/proc/net/dev
    NET_INTERFACE *p_interface;  // 本机网络接口信息结构体
    NET_SPEED net_speed;
    int nums;  // 网卡数量
    void open_netconf(FILE **fd);
    void thread_net();
    int get_interface_info(NET_INTERFACE **net, int *n);
    void show_netinterfaces(NET_INTERFACE *p_net, int n);
    void get_rtx_bytes(char *name, RTX_BYTES *rtx);
    void cal_netinterface_speed(double *u_speed, double *d_speed,
                                unsigned char *level, RTX_BYTES *rtx0,
                                RTX_BYTES *rtx1);
    void get_network_speed(NET_INTERFACE *p_net);
    NET_SPEED get_total_network_speed(NET_INTERFACE *p_net);

};

#endif // NETSPEED_H