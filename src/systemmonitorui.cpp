#include "systemmonitorapp.h"
#include <iostream>
#include <string>
#include "settings.h"



void SystemMonitorApp::initUI()
{
    this->resize(300, 100);
    this->setFixedSize(260, 60);
    this->setMenu();

    setWindowFlags(Qt::Tool); // 设置为工具窗口，取消 状态栏/任务栏 显示图标
    setWindowOpacity(1.0);  // 窗口不透明度
    setWindowFlag(Qt::FramelessWindowHint); // 取消显示标题栏
    setAttribute(Qt::WA_TranslucentBackground, true);

    setWindowFlag(Qt::WindowStaysOnTopHint);  // 显示在最上层
    setAttribute(Qt::WA_QuitOnClose, true);

    this->createLabel();

    settings_dialog = new SettingsDialog(this); // 设置界面
    config_data = settings_dialog->config_data;
    setConfigJson();
}
void SystemMonitorApp::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void SystemMonitorApp::createLabel()
{
    QSize tsize = this->size();
    int w = tsize.width();
    int h = tsize.height();
    // 上传速度
    label_up = new QLabel(this);
    label_up->setObjectName("label_up");
    label_up_speed = new QLabel(this);
    label_up_speed->setObjectName("label_up_speed");
    // 下载速度
    label_down = new QLabel(this);
    label_down->setObjectName("label_down");
    label_down_speed = new QLabel(this);
    label_down_speed->setObjectName("label_down_speed");
    // cpu 占用率/使用率
    label_cpu = new QLabel(this);
    label_cpu->setObjectName("label_cpu");
    label_cpu_usage_rate = new QLabel(this);
    label_cpu_usage_rate->setObjectName("label_cpu_usage_rate");
    // memory 占用率/使用率
    label_memory = new QLabel(this);
    label_memory->setObjectName("label_memory");
    label_memory_usage_rate = new QLabel(this);
    label_memory_usage_rate->setObjectName("label_memory_usage_rate");

    int shift_left = 12;
    int shift_top = 0;
    const int step_top = 2;
    const int step = 2;
    int shift = shift_left + step;
    QSize up_down_size(40, 20);
    // up speed
    label_up->setText(translate("monitor", "Up") +":");
    label_up->setAlignment(Qt::AlignRight);
    label_up->setFixedWidth(up_down_size.width());
    // label_up->setFixedSize(up_down_size);
    label_up->move(QPoint(shift_left, shift_top+step_top));
    label_up_speed->setText("0KB/s");
    // label_up_speed->setFixedSize(65, 20);
    label_up_speed->move(QPoint(label_up->width()+shift, shift_top));

    // down speed
    int t = 6;
    QSize down_size(up_down_size.width()+t, 20);
    int s_left = shift_left-t;
    label_down->setText(translate("monitor", "Down") + ":");
    label_down->setAlignment(Qt::AlignRight);
    label_down->setFixedWidth(down_size.width());
    label_down->move(s_left, h/2+shift_top+step_top);
    label_down_speed->setText("0KB/s");
    s_left = shift-t;
    label_down_speed->move(QPoint(label_down->width()+s_left, h/2+shift_top));

    // CPU
    shift_left = 15;
    shift = shift_left + step;
    QSize cpu_size(40, 20);
    label_cpu->setText("CPU:");
    label_cpu->setAlignment(Qt::AlignRight);
    label_cpu->setFixedWidth(cpu_size.width());
    label_cpu->move(w/2+shift_left, shift_top+step_top);
    label_cpu_usage_rate->setText("0%");
    label_cpu_usage_rate->move(w/2+cpu_size.width()+shift, shift_top);

    // memory
    QSize memory_size(40, 20);
    label_memory->setText(translate("monitor", "MEM") + ":");
    label_memory->setFixedWidth(memory_size.width());
    label_memory->setAlignment(Qt::AlignRight);
    label_memory->move(w/2+shift_left, h/2+shift_top+step_top);
    label_memory_usage_rate->setText("0%");
    label_memory_usage_rate->move(w/2+memory_size.width()+shift, h/2+shift_top);
}
/**
 * 设置图标
 * 启动后在工具栏显示图标
 */
void SystemMonitorApp::setIcon()
{
    // 应用程序左上角的图标、状态栏的图标、可执行文件本身的图标，Linux下可以同时设置
    setWindowIcon(QIcon(QString::fromStdString(config_data["windowIcon"])));

    // 设置显示在 通知栏/工具栏/菜单栏（指的是同一个地方） 里的图标
    trayIcon->setIcon(QIcon(QString::fromStdString(config_data["toolIcon"])));
    trayIcon->show(); // 在菜单兰（ubuntu中的位置是桌面右上角）中显示
}
/**
 * 菜单
 */
void SystemMonitorApp::setMenu()
{
    /* 设置通知栏程序的菜单 */
    QMenu *menu = new QMenu();
    actionExit = new QAction(translate("monitor", "Exit"), menu); // 菜单中的退出选项
    actionSettings = new QAction(translate("monitor", "Settings"), menu);  // 设置选项
    actionReset = new QAction(translate("monitor", "Restore default configuration"), menu);  // 恢复默认配置选项
    menu->addAction(actionReset);
    menu->addAction(actionSettings);
    menu->addAction(actionExit);
    trayIcon->setContextMenu(menu); // 添加菜单
}
/**
 * 修改文字颜色
 */
void SystemMonitorApp::changeColor()
{
    QFont font;
    label_up;
    label_up_speed->setFont(font);
}
void SystemMonitorApp::set_SystemMonitorApp_Style()
{
    QString border_color = strToQstring(config_data["SysAppStyle"]["border"]);
    QString border_radius = strToQstring(config_data["SysAppStyle"]["border-radius"]);
    if(config_data["SysAppStyle"]["border_color"] == false){
        border_color = "";
    }
    if(config_data["SysAppStyle"]["border-radius"] == false){
        border_radius = "";
    }
    
    QString qstr = strToQstring(config_data["SysAppStyle"]["begin"]) +
    border_color + border_radius +
    strToQstring(config_data["SysAppStyle"]["bkgImage"]) +
    strToQstring(config_data["SysAppStyle"]["bkgColor"]) +
    strToQstring(config_data["SysAppStyle"]["end"]);
    this->setStyleSheet(qstr);
}
void SystemMonitorApp::set_Syslabel_Style()
{
    QString qstr = strToQstring(config_data["labelStyle"]["begin"]) +
    strToQstring(config_data["labelStyle"]["fontColor"]) +
    strToQstring(config_data["labelStyle"]["fontSize"]) +
    strToQstring(config_data["labelStyle"]["end"]);
    this->label_cpu->setStyleSheet(qstr);
    this->label_cpu_usage_rate->setStyleSheet(qstr);
    this->label_memory->setStyleSheet(qstr);
    this->label_memory_usage_rate->setStyleSheet(qstr);
    this->label_up->setStyleSheet(qstr);
    this->label_up_speed->setStyleSheet(qstr);
    this->label_down->setStyleSheet(qstr);
    this->label_down_speed->setStyleSheet(qstr);
}
void SystemMonitorApp::setPosition()
{
    int x = config_data["position"]["x"];
    int y = config_data["position"]["y"];
    this->move(x, y);
}
/**
 * 更新配置文件
 */
void SystemMonitorApp::update_UI()
{
    config_data = settings_dialog->config_data;
    setConfigJson();
}
void SystemMonitorApp::setConfigJson()
{
    setIcon();
    set_SystemMonitorApp_Style();
    set_Syslabel_Style();
    setPosition();
}
/**
 * 恢复默认配置
 */
void SystemMonitorApp::resetConfigJson()
{
    QMessageBox mbox;
    moveToCenter((QWidget*)&mbox);
    int ret = mbox.information(NULL,
                                translate("monitor", "Prompt"),  // 提示
                                translate("monitor", "Are you sure you want to restore the default configuration?"),  // 确定要恢复默认配置？
                                QMessageBox::Cancel|QMessageBox::Ok,QMessageBox::Ok);

    if(ret == QMessageBox::Cancel) // 取消恢复默认配置
    {
        qDebug() << "QMessageBox: " << "取消恢复默认配置" << ret ;
        return;
    }
    qDebug() << "QMessageBox: " << "恢复默认配置" << ret ;

    copyFile(default_path, config_path);
    read_config_json(config_path, config_data);
    setConfigJson();
    settings_dialog->config_data =settings_dialog->tmp_config_data = config_data;
}