#include "systemmonitorapp.h"

#include <QApplication>
#include <QCoreApplication>

// 获取可执行文件的绝对路径
std::string getExecutablePath() {
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len < 0 || (size_t)len >= sizeof(buf)) {
        throw std::runtime_error("Failed to readlink /proc/self/exe");
    }
    buf[len] = '\0';
    return std::string(buf);
}


int main(int argc, char *argv[])
{
    std::string executablePath;
    try {
        executablePath = getExecutablePath();
        std::cout << "Executable path: " << executablePath << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    QString parent_dir = SystemMonitorApp::getParentDir(QString::fromStdString(executablePath));
    QString qt_plugins = parent_dir + QDir::separator() + "qt_plugins";
    std::cout << "qt_plugins: " <<  qt_plugins.toStdString() << std::endl;
    setenv("QT_PLUGIN_PATH", qt_plugins.toStdString().c_str(), 1);

    QApplication a(argc, argv);
    SystemMonitorApp w;
    w.show();
    return a.exec();
}
