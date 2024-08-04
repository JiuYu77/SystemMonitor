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

/**
 * @description: 打开网络接口设备文件/proc/net/dev
 * @param {FILE} *
 * @return {*}
 */
void open_netconf(FILE **fd);
/**
 * @description: 获取网卡数量和IP地址
 * @param {NET_INTERFACE} * 
 * @param {int} *n 本机网卡数量
 * @return {*}
 */
int get_interface_info(NET_INTERFACE **net, int *n);
/**
 * @description: 显示本机活动网络接口
 * @param {NET_INTERFACE} *p_net
 * @return {*}
 */
void show_netinterfaces(NET_INTERFACE *p_net, int n);
/**
 * @description: 获取网卡当前时刻的收发包信息
 * @param {char} *name
 * @param {RTX_BYTES} *rtx
 * @return {*}
 */
void get_rtx_bytes(char *name, RTX_BYTES *rtx);
/**
 * @description: 计算网卡的上下行网速
 * @param {double} *u_speed
 * @param {double} *d_speed
 * @param {unsignedchar} *level
 * @param {RTX_BYTES} *rtx0
 * @param {RTX_BYTES} *rtx1
 * @return {*}
 */
void cal_netinterface_speed(double *u_speed, double *d_speed,
                            unsigned char *level, RTX_BYTES *rtx0,
                            RTX_BYTES *rtx1);
/**
 * @description: 获取主机网卡速度信息
 * @param {NET_INTERFACE} *p_net
 * @return {*}
 */
void get_network_speed(NET_INTERFACE *p_net);
NET_SPEED get_total_network_speed(NET_INTERFACE *p_net);


class NetSpeed
{
public:
    NetSpeed();
    ~NetSpeed();
    NET_INTERFACE *p_interface;
    NET_SPEED net_speed;
    int nums;  // 网卡数量
    void thread_net();
    int get_interface_info(NET_INTERFACE **net, int *n);

};

#endif // NETSPEED_H