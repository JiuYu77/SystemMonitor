#include "systemmonitorapp.h"
#include "settings.h"


SystemMonitorApp::SystemMonitorApp(QWidget *parent)
    // : QMainWindow(parent)
    : Win(parent)
{
    setConfigPath();
    read_config_json(config_path, config_data);
    initLanguage();  // 设置语言，要在创建控件之前设置;
    initUI();
    setConnect();
    startThread();
}

SystemMonitorApp::~SystemMonitorApp()
{
    stopThread();
    Exit();
}

void SystemMonitorApp::setConnect()
{
    connect(actionExit, &QAction::triggered, this, &SystemMonitorApp::Exit);  // 退出程序
    connect(actionSettings, &QAction::triggered, settings_dialog, &SettingsDialog::show);  // 显示设置界面
    connect(actionReset, &QAction::triggered, this, &SystemMonitorApp::resetConfigJson);
}

// 鼠标拖动窗口
void SystemMonitorApp::mousePressEvent(QMouseEvent* event)
{
    mouse_pos = event->globalPos();
    window_pos = this->pos();
    diff_pos = mouse_pos - window_pos;
}
// 鼠标拖动窗口
void SystemMonitorApp::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->globalPos();
    QPoint point = pos - diff_pos;
    this->move(point);
    config_data["position"]["x"] = point.x();
    config_data["position"]["y"] = point.y();
    writeJsonFile(config_path, config_data);  // 更新位置
}
// 启动线程
void SystemMonitorApp::startThread()
{
    labels = {label_up_speed, label_down_speed, label_cpu_usage_rate, label_memory_usage_rate};

    pthread_create(&thread_cpu_mem, NULL, &update_cpu_mem, &labels);
    pthread_create(&thread_speed, NULL, &update_speed, &labels);
}
// 结束/取消 线程
void SystemMonitorApp::stopThread()
{
    pthread_cancel(thread_cpu_mem);
    // pthread_join(thread_speed, NULL); // 无法立即结束线程
    pthread_cancel(thread_speed);
}
// 更新 CPU, 内存
void* SystemMonitorApp::update_cpu_mem(void* arg)
{
    CpuMemory cpu_mem;
    Labels* labels = (Labels*)arg;

    while(1)
    {
        cpu_mem.thread_core();
        QString str_cpu = QString::number(cpu_mem.cpu_used, 'f', 2) + '%';
        labels->cpu->setText(str_cpu);
        QString str_mem = QString::number(cpu_mem.mem_used, 'f', 2) + '%';
        labels->memory->setText(str_mem);
        sleep(1);
    }
    return NULL;
}
// 更新 网速
void* SystemMonitorApp::update_speed(void* arg)
{
    NET_SPEED net_speed;
    Labels* labels = (Labels*)arg;

    while(1)
    {
        // 网速
        net_speed = thread_net();
        labels->down->setText(QString::fromStdString(net_speed.d_speed_str));
        labels->up->setText(QString::fromStdString(net_speed.u_speed_str));
        sleep(1);
    }
    return NULL;
}
/**
 * 配置文件路径
 */
void SystemMonitorApp::setConfigPath()
{
    // 获取可执行文件的路径
    executablePath = QCoreApplication::applicationDirPath();
    QDir dir(executablePath);
    qDebug() << "Executable Path:" << executablePath;
    if (dir.cdUp()) {
        parentDir = dir.absolutePath();
        qDebug() << "Parent directory:" << parentDir;
    } else {
        qDebug() << "Unable to go up one level in the directory hierarchy.";
    }
    q_parentDir = QDir(parentDir);
    // 获取资源文件的路径
    // QString path = QDir(parentDir).absoluteFilePath("config/default.json");
    QString path = q_parentDir.absoluteFilePath("config/default.json");
    default_path = qstrToStdString(path);
    // QString path2 = QDir(parentDir).absoluteFilePath("config/config.json");
    QString path2 = q_parentDir.absoluteFilePath("config/config.json");
    config_path = qstrToStdString(path2);
    qDebug() << "default.json directory:" << path;
    qDebug() << "config.json  directory:" << path2;
}
//  设置语言
void SystemMonitorApp::initLanguage()
{
    firstLanguage();
    std::string lang = config_data["Language"]["language"];
    int index = config_data["Language"]["index"];
    if(index == 0)
    {
        translator.load(":/res/language/SystemMonitor_zh_CN.qm");
        qDebug() << "Simplified Chinese...";
    }else if(index == 1){
        translator.load(":/res/language/SystemMonitor_en_US.qm");
        qDebug() << "English...";
    }
    // translator.load(":/res/language/SystemMonitor_en_US.qm");
    QCoreApplication::installTranslator(&translator);
    qDebug() << "initLanguage()...";
}
//  设置语言
void SystemMonitorApp::firstLanguage()
{
    bool flag = config_data["Language"]["flag"];
    if(flag == true)
    {
        return;
    }
    flag = false;
    std::string lang;
    int index;
    QLocale::Language lab = QLocale::system().language();
    if(QLocale::Chinese == lab)
    {
        translator.load(":/res/language/SystemMonitor_zh_CN.qm");
        lang = "中文(简体)";
        index = 0;
        qDebug() << "Simplified Chinese...";
    }else if(QLocale::English == lab){
        translator.load(":/res/language/SystemMonitor_en_US.qm");
        lang = "English";
        index = 1;
        qDebug() << "English...";
    }
    QCoreApplication::installTranslator(&translator);
    config_data["Language"]["flag"] = flag;
    config_data["Language"]["language"] = lang;
    config_data["Language"]["index"] = index;
    writeJsonFile(config_path, config_data);  // 更新config/config.json
    qDebug() << "initLanguage()...";
}
/**
 * 释放内存
 */
void SystemMonitorApp::del()
{
    delete settings_dialog;
}
void SystemMonitorApp::Exit()
{
    stopThread();  // 先结束线程
    del();
    close();
}
/**
 * 重启程序
 */
void SystemMonitorApp::restart()
{
    QProcess::startDetached(QApplication::applicationFilePath(), QApplication::arguments());
    Exit();
}