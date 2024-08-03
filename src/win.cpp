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
