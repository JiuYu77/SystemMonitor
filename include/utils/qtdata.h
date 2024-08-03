#ifndef QTDATA_H
#define QTDATA_H

#include <QString>

// std::string转QString
QString strToQstring(std::string str);
// QString转std::string
std::string qstrToStdString(QString qstr);

#endif // QTDATA_H
