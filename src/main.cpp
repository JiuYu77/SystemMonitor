#include "systemmonitorapp.h"

#include <QApplication>
#include <QCoreApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SystemMonitorApp w;
    w.show();
    return a.exec();
}
