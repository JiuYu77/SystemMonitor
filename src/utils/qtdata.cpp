#include "utils/qtdata.h"

QString strToQstring(std::string str)
{
    return QString::fromStdString(str);
}
std::string qstrToStdString(QString qstr)
{
    return qstr.toStdString();
}