#ifndef WIN_H
#define WIN_H

#include <QMainWindow>
#include <QFile>
#include <QCoreApplication>
#include <QDir>

#include <QDebug>

// #include "utils/qtdata.h"

class Win : public QMainWindow
{
    Q_OBJECT

public:
    Win(QWidget *parent = nullptr);
    ~Win();
    QString executable_path;  // 可执行文件所在目录
    QString parent_dir;  // 可执行文件父目录
    QDir qdir_parentDir;
    void setSheet(QString filename);
    static QString getAppDir();
    static QString getParentDir(QString path);
    void setPath();

private:

};
#endif // WIN_H
