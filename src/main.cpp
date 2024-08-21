#include "systemmonitorapp.h"

#include <QApplication>
#include <QSettings>


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

void set_QT_PLUGIN_PATH()
{
    std::string executablePath;
    try {
        executablePath = getExecutablePath();
        std::cout << "Executable path: " << executablePath << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    QString parent_dir = SystemMonitorApp::getParentDir(QString::fromStdString(executablePath));
    std::cout << "parent_dir: " <<  parent_dir.toStdString() << std::endl;
    QString qt_plugins = parent_dir + QDir::separator() + "qt_plugins";

    std::cout << "qt_plugins: " <<  qt_plugins.toStdString() << std::endl;
    setenv("QT_PLUGIN_PATH", qt_plugins.toStdString().c_str(), 1);
    std::cout << "set_QT_PLUGIN_PATH ... " << std::endl;


    QString qt_plugins_platforms = parent_dir + QDir::separator() + "qt_plugins" + QDir::separator() + "latforms";
    std::cout << "qt_plugins_platforms: " <<  qt_plugins_platforms.toStdString() << std::endl;
    setenv("QT_QPA_PLATFORM_PLUGIN_PATH", qt_plugins_platforms.toStdString().c_str(), 1);
    std::cout << "set_QT_QPA_PLATFORM_PLUGIN_PATH ... " << std::endl;
}

int main(int argc, char *argv[])
{
    // QLibrarySettings;
    // QString qtconfig = QStringLiteral(":/qt.conf");
    // if (QFile::exists(qtconfig))
    //     return new QSettings(qtconfig, QSettings::IniFormat);


    set_QT_PLUGIN_PATH();

    // size_t i = executablePath.find_last_of('/');
    // std::string parent_dir = executablePath.substr(0, i);
    // std::cout << "parent_dir: " <<  parent_dir << std::endl;
    // std::string qt_plugins = parent_dir + QString(QDir::separator()).toStdString() + "qt_plugins";
    // std::cout << "qt_plugins: " <<  qt_plugins << std::endl;
    // setenv("QT_PLUGIN_PATH", qt_plugins.c_str(), 1);


    QApplication a(argc, argv);
    std::cout << "app start. " << std::endl;
    SystemMonitorApp w;
    w.show();
    return a.exec();
}
