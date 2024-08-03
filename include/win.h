#ifndef WIN_H
#define WIN_H

#include <QMainWindow>
#include <QFile>
#include "utils/qtdata.h"

class Win : public QMainWindow
{
    Q_OBJECT

public:
    Win(QWidget *parent = nullptr);
    ~Win();
    void setSheet(QString filename);
private:

};
#endif // WIN_H
