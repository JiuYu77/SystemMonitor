#include "monitor/netspeed.h"


NetSpeed::NetSpeed()
{
    p_interface = NULL;
    p_interface = (NET_INTERFACE *)malloc(sizeof(NET_INTERFACE));
    nums = 0;  // 网卡数量
    get_interface_info(&p_interface, &nums);  //网卡结构体指针初始化
}
NetSpeed::~NetSpeed()
{}
/**
 * @description: 打开网络接口设备文件/proc/net/dev
 * @param {FILE} *
 * @return {*}
 */
void NetSpeed::open_netconf(FILE **fd)
{
    *fd = fopen("/proc/net/dev", "r");
    if (*fd == NULL) {
        perror("open file /proc/net/dev failed!\n");
        exit(0);
    }
}
/**
 * @description: 网络信息监控线程
 * @param {*}
 * @return {*}
 */
void NetSpeed::thread_net()
{
    // printf("%s\n", __FUNCTION__);
    get_network_speed(p_interface);
    // show_netinterfaces(p_interface, 1);
    net_speed = get_total_network_speed(p_interface);
}
/**
 * @description: 获取网卡数量和IP地址
 * @param {NET_INTERFACE} * 
 * @param {int} *n 本机网卡数量
 * @return {*}
 */
int NetSpeed::get_interface_info(NET_INTERFACE **net, int *n)
{
    int fd;
    int num = 0;
    struct ifreq buf[16];
    struct ifconf ifc;

    NET_INTERFACE *p_temp = NULL;
    (*net)->next = NULL;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        close(fd);
        printf("socket open failed\n");
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;

    if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc)) {
        // get interface nums
        num = ifc.ifc_len / sizeof(struct ifreq);
        *n = num;

        // find all interfaces;
        while (num-- > 0) {
            // exclude lo interface
            /* if (!strcmp("lo", buf[num].ifr_name))
            continue; */

            // get interface name
            strcpy((*net)->name, buf[num].ifr_name);
#if DEBUG
            printf("name:%8s\t", (*net)->name);
#endif
            // get the ipaddress of the interface
            if (!(ioctl(fd, SIOCGIFADDR, (char *)&buf[num]))) {

                memset((*net)->ip, 0, 16);
                strcpy(
                    (*net)->ip,
                    inet_ntoa(((struct sockaddr_in *)(&buf[num].ifr_addr))->sin_addr));
#if DEBUG
                printf("IP:%16s\t", (*net)->ip);
#endif
            }

            // get the mac of this interface
            if (!ioctl(fd, SIOCGIFHWADDR, (char *)(&buf[num]))) {

                memset((*net)->mac, 0, 13);

                snprintf((*net)->mac, 13, "%02x%02x%02x%02x%02x%02x",
                         (unsigned char)buf[num].ifr_hwaddr.sa_data[0],
                         (unsigned char)buf[num].ifr_hwaddr.sa_data[1],
                         (unsigned char)buf[num].ifr_hwaddr.sa_data[2],
                         (unsigned char)buf[num].ifr_hwaddr.sa_data[3],
                         (unsigned char)buf[num].ifr_hwaddr.sa_data[4],
                         (unsigned char)buf[num].ifr_hwaddr.sa_data[5]);
#if DEBUG
                printf("mac:%12s\n", (*net)->mac);
#endif
            }
            if (num >= 1) {
                p_temp = (NET_INTERFACE *)malloc(sizeof(NET_INTERFACE));
                memset(p_temp, 0, sizeof(NET_INTERFACE));
                p_temp->next = *net;
                *net = p_temp;
            }
        }
        return 0;
    } else {
        return -1;
    }
}
/**
 * @description: 获取网卡当前时刻的收发包信息
 * @param {char} *name
 * @param {RTX_BYTES} *rtx
 * @return {*}
 */
void NetSpeed::get_rtx_bytes(char *name, RTX_BYTES *rtx)
{
    char *line = NULL;

    size_t bytes_read;
    size_t read;

    char str1[32];
    char str2[32];

    int i = 0;
    open_netconf(&net_dev_file);
    //获取时间
    gettimeofday(&rtx->rtx_time, NULL);
    //从第三行开始读取网络接口数据
    while ((read = getline(&line, &bytes_read, net_dev_file)) != -1) {
        if ((++i) <= 2)
            continue;
        if (strstr(line, name) != NULL) {
            memset(str1, 0x0, 32);
            memset(str2, 0x0, 32);

            sscanf(line, "%*s%s%*s%*s%*s%*s%*s%*s%*s%s", str1, str2);

            rtx->tx_bytes = atol(str2);
            rtx->rx_bytes = atol(str1);
#if DEBUG
            printf("name:%16s\t tx:%10ld\trx:%10ld\n", name, rtx->tx_bytes,
                   rtx->rx_bytes);
#endif
	}
    }

    free(line);
    fclose(net_dev_file);
}
/**
 * @description: 计算网卡的上下行网速
 * @param {double} *u_speed
 * @param {double} *d_speed
 * @param {unsignedchar} *level
 * @param {RTX_BYTES} *rtx0
 * @param {RTX_BYTES} *rtx1
 * @return {*}
 */
void NetSpeed::cal_netinterface_speed(double *u_speed, double *d_speed,
                            unsigned char *level, RTX_BYTES *rtx0,
                            RTX_BYTES *rtx1)
{
    long int time_lapse;

    time_lapse = (rtx1->rtx_time.tv_sec * 1000 + rtx1->rtx_time.tv_usec / 1000) -
                 (rtx0->rtx_time.tv_sec * 1000 + rtx0->rtx_time.tv_usec / 1000);

    *d_speed = (rtx1->rx_bytes - rtx0->rx_bytes) * 1.0 /
               (1024 * time_lapse * 1.0 / 1000);
    *u_speed = (rtx1->tx_bytes - rtx0->tx_bytes) * 1.0 /
               (1024 * time_lapse * 1.0 / 1000);

    // speed_level 置0
    //    *level &= 0x00;

    if (*d_speed >= MB * 1.0) {
        *d_speed = *d_speed / 1024;
        *level |= 0x1;
#if DEBUG
        printf("download:%10.2lfMB/s\t\t", *d_speed);
#endif
    } else {
        //定义速度级别
        *level &= 0xFE;
#if DEBUG
        printf("download:%10.2lfKB/s\t\t", *d_speed);
#endif
    }

    if (*u_speed >= MB * 1.0) {
        *u_speed = *u_speed / 1024;
        *level |= 0x1 << 1;
#if DEBUG
        printf("upload:%10.2lfMB/s\n", *u_speed);
#endif
    } else {
        //定义速度级别
        *level &= 0xFD;
#if DEBUG
        printf("upload:%10.2lfKB/s\n", *u_speed);
#endif
    }
}
/**
 * @description: 获取主机网卡速度信息
 * @param {NET_INTERFACE} *p_net
 * @return {*}
 */
void NetSpeed::get_network_speed(NET_INTERFACE *p_net)
{

    RTX_BYTES rtx0, rtx1;

    NET_INTERFACE *temp1, *temp2, *temp3;
    temp1 = p_net;
    temp2 = p_net;
    temp3 = p_net;
    while (temp1 != NULL) {

        get_rtx_bytes(temp1->name, &rtx0);
        temp1->rtx0_cnt.tx_bytes = rtx0.tx_bytes;
        temp1->rtx0_cnt.rx_bytes = rtx0.rx_bytes;
        temp1->rtx0_cnt.rtx_time = rtx0.rtx_time;
        temp1 = temp1->next;
    }

    sleep(WAIT_SECOND);

    while (temp2 != NULL) {

        get_rtx_bytes(temp2->name, &rtx1);
        temp2->rtx1_cnt.tx_bytes = rtx1.tx_bytes;
        temp2->rtx1_cnt.rx_bytes = rtx1.rx_bytes;
        temp2->rtx1_cnt.rtx_time = rtx1.rtx_time;

        temp2->speed_level &= 0x00;
        temp2 = temp2->next;
    }
    // temp->speed_level &= 0x00;
    while (temp3 != NULL) {
        cal_netinterface_speed(&temp3->u_speed, &temp3->d_speed,
                               &temp3->speed_level, (&temp3->rtx0_cnt),
                               (&temp3->rtx1_cnt));
        temp3 = temp3->next;
    }
}
/**
 * @description: 显示本机活动网络接口
 * @param {NET_INTERFACE} *p_net
 * @return {*}
 */
void NetSpeed::show_netinterfaces(NET_INTERFACE *p_net, int n)
{
    NET_INTERFACE *temp;
    temp = p_net;

    while (temp != NULL) {
        if (!n) {
            printf("Interface: %-16s\t IP:%16s\t MAC:%12s\n", temp->name, temp->ip,
                   temp->mac);
        } else {
            printf("Interface: %-16s\t", temp->name);
            if (temp->speed_level & 0x1) {
                printf("download:%10.2lfMB/s\t\t", temp->d_speed);
                temp->bool_down = true;
            } else {
                printf("download:%10.2lfKB/s\t\t", temp->d_speed);
                temp->bool_down = false;
            }

            if ((temp->speed_level >> 1) & 0x1) {
                printf("upload:%10.2lfMB/s\n", temp->u_speed);
                temp->bool_up = true;
            } else {
                printf("upload:%10.2lfKB/s\n", temp->u_speed);
                temp->bool_up = false;
            }
        }

        temp = temp->next;
    }
}


NET_SPEED NetSpeed::get_total_network_speed(NET_INTERFACE *p_net)
{
    NET_INTERFACE *temp = p_net;
    NET_SPEED net_speed = {0, 0};
    while (temp != NULL)
    {
        if (temp->speed_level & 0x1) { /*下行速度*/
                // printf("download:%10.2lfMB/s\t\t", temp->d_speed);
                net_speed.d_speed += temp->d_speed * 1024;
        } else {
                // printf("download:%10.2lfKB/s\t\t", temp->d_speed);
                net_speed.d_speed += temp->d_speed;
        }

        if ((temp->speed_level >> 1) & 0x1) { /*上行速度*/
            // printf("upload:%10.2lfMB/s\n", temp->u_speed);
            net_speed.u_speed += temp->u_speed * 1024;
        } else {
            // printf("upload:%10.2lfKB/s\n", temp->u_speed);
            net_speed.u_speed += temp->u_speed;
        }
        temp = temp->next;
    }

    if(net_speed.d_speed > 1024.0){
        net_speed.d_speed_str = doubleToString(net_speed.d_speed / 1024, 2) + "MB/s";
    }else{
        net_speed.d_speed_str = doubleToString(net_speed.d_speed, 2) + "KB/s";
    }
    if(net_speed.u_speed > 1024.0){
        net_speed.u_speed_str = doubleToString(net_speed.u_speed / 1024, 2) + "MB/s";
    }else{
        net_speed.u_speed_str = doubleToString(net_speed.u_speed, 2) + "KB/s";
    }
    return net_speed;
}
