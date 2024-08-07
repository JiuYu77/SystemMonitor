#include "systemmonitorapp.h"

#include <QApplication>
#include <QCoreApplication>


int main(int argc, char *argv[])
{
    std::string currentFilePath(__FILE__);  // 当前文件路径: /.../src/main.cpp

    std::cout << "当前文件路径: " << currentFilePath << std::endl;
    QString parent_dir = SystemMonitorApp::getParentDir(QString::fromStdString(currentFilePath));
    std::cout << "父路径: " << parent_dir.toStdString() << std::endl;
    QString pparent_dir = SystemMonitorApp::getParentDir(parent_dir);
    QString lib_path = pparent_dir + "/lib";
    std::cout << "lib路径: " << lib_path.toStdString() << std::endl;
    
    // 设置 Qt plugin 环境变量: QT_QPA_PLATFORM_PLUGIN_PATH 或 QT_PLUGIN_PATH 都可以
    setenv("QT_QPA_PLATFORM_PLUGIN_PATH", lib_path.toStdString().c_str(), 1);

    QApplication a(argc, argv);

    SystemMonitorApp w;
    w.show();
    return a.exec();
}
