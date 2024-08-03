#ifndef QTUI_H
#define QTUI_H

#include <QScreen>
#include <QGuiApplication>
#include <QPoint>
#include <QWidget>
#include <QPalette>
#include <QPushButton>
#include <QApplication>


// 获取窗口居中的点
QPoint getCenterLeftTopPoint(int w, int h);
// 将窗口移动到屏幕中间
void moveToCenter(QWidget *widget);
// 基于当前坐标，进行移动
void moveToPos(QWidget *w, int deltaX, int deltaY);
// 让按钮显示颜色
void setQPushButtonColor (QPushButton *btn);
// 翻译标注
QString translate(const char * context, const char * key, const char * disambiguation = nullptr, int n = -1);


#endif // QTUI_H
