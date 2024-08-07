#include "systemmonitorapp.h"

#include <QApplication>
#include <QCoreApplication>


int main(int argc, char *argv[])
{
    QString app_dir = SystemMonitorApp::getAppDir();
    QString parent_dir = SystemMonitorApp::getParentDir(app_dir);
    QString lib_path = parent_dir + "/lib";
    // 设置 Qt plugin 环境变量: QT_QPA_PLATFORM_PLUGIN_PATH 或 QT_PLUGIN_PATH 都可以
    setenv("QT_QPA_PLATFORM_PLUGIN_PATH", lib_path.toStdString().c_str(), 1);
    // setenv("QT_PLUGIN_PATH", "../lib", 1);
    // setenv("QT_QPA_PLATFORM_PLUGIN_PATH", "../lib", 1);

    QApplication a(argc, argv);
    SystemMonitorApp w;
    w.show();
    return a.exec();
}
