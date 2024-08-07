#include "win.h"

Win::Win(QWidget *parent)
    : QMainWindow(parent)
{
}

Win::~Win()
{
}

void Win::setSheet(QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);//读取qss文件，设置样式
    if(file.isOpen())
    {
        QString qss = file.readAll();
        this->setStyleSheet(qss);
    }
    file.close();
}
/**
 * 获取可执行文件的路径
 */
QString Win::getAppDir()
{
    QString executable_path = QCoreApplication::applicationDirPath();
    return executable_path;
}
/**
 * 获取父目录
 */
QString Win::getParentDir(QString path)
{
    QDir dir(path);
    QString parent_dir;
    if (dir.cdUp()) {
        parent_dir = dir.absolutePath();
        qDebug() << "Parent directory:" << parent_dir;
    } else {
        qDebug() << "Unable to go up one level in the directory hierarchy.";
    }
    return parent_dir;
}
/**
 * 设置程序目录
 */
void Win::setPath()
{
    executable_path = getAppDir();
    qDebug() << "Executable Path:" << executable_path;
    this->parent_dir = getParentDir(executable_path);
    qdir_parentDir = QDir(parent_dir);
}