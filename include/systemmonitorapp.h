#ifndef SYSTEMMONITORAPP_H
#define SYSTEMMONITORAPP_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDir>
#include <QTranslator>
#include <QProcess>


#include "win.h"
// #include "monitor/cpu_mem.h"
// #include "monitor/netspeed.h"
#include "monitor/monitor.h"
#include "utils/json.h"
#include "utils/qtui.h"


#include <pthread.h>


#include <QDebug>

class SettingsDialog;

struct Labels{
    QLabel *up;
    QLabel *down;
    QLabel *cpu;
    QLabel *memory;
};

class SystemMonitorApp : public Win
{
    Q_OBJECT

public:
    SystemMonitorApp(QWidget *parent = nullptr);
    ~SystemMonitorApp();
    void update_UI();  // json; 更新界面

    std::string default_path;
    std::string config_path;
    QTranslator translator;
    void restart();


private:
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    QAction *actionExit;
    QAction *actionSettings;
    QAction *actionReset;
    void initUI();
    void setConfigPath();
    void setIcon();
    void setMenu();
    void paintEvent(QPaintEvent *event);  // 去除圆角，周围的四个直角
    void setConnect();  // 连接信号和槽函数
    void initLanguage();  // 设置软件语言
    void firstLanguage();
    void del();  // 释放内存
    void Exit();

    QLabel *label_up, *label_up_speed; // 上传(上行)速度
    QLabel *label_down, *label_down_speed;  // 下载(下行)速度
    QLabel *label_cpu, *label_cpu_usage_rate;  // CPU 使用率
    QLabel *label_memory, *label_memory_usage_rate;  // 内存 使用率
    Labels labels;
    void createLabel();
    void changeColor();

    /** 鼠标拖动窗口 **/
    QPoint diff_pos;   // 鼠标和窗口的相对位移
    QPoint window_pos; // 窗口的绝对位置
    QPoint mouse_pos;  // 鼠标的绝对位置
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    /** 鼠标拖动窗口 **/

    SettingsDialog *settings_dialog; // 设置界面

    /** 多线程 **/
    pthread_t thread_cpu_mem;
    pthread_t thread_speed;
    void startThread();
    void stopThread();
    /** 多线程 **/

    static void* update_cpu_mem(void* arg);    // 更新
    static void* update_speed(void* arg);
    void update_up_speed();           // 更新 上传(上行)速度
    void update_dowm_speed();         // 更新 下载(下行)速度
    static void* update_cpu_usage_rate(void* arg);     // 更新 CPU 使用率
    void update_memory_usage_rate();  // 更新 内存 使用率

    /** json **/
    json config_data;
    void set_SystemMonitorApp_Style();
    void set_Syslabel_Style();
    void setPosition();
    void setConfigJson();  // json配置文件,设置界面
    void resetConfigJson();  // 恢复默认配置
    /** json **/
};
#endif // SYSTEMMONITORAPP_H
