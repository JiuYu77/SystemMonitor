#include "utils/qtui.h"


/**
 * 获取窗口居中的点
 */
QPoint getCenterLeftTopPoint(int w, int h)
{
    // 获取主屏幕
    QScreen *screen = QGuiApplication::primaryScreen();
    // 获取主屏幕的分辨率
    QRect screenGeometry = screen->geometry();
    QPoint p((screenGeometry.width() - w) / 2, (screenGeometry.height() - h) / 2);
    return p;
}
void moveToCenter(QWidget *widget)
{
    QPoint p = getCenterLeftTopPoint(widget->width(), widget->height());
    widget->move(p);
}
/**
 * 基于当前坐标，进行移动
 */
void moveToPos(QWidget *w, int deltaX, int deltaY)
{
    QPoint pos = w->pos();
    if(deltaX != 0)
        pos.setX(pos.x() + deltaX);
    if(deltaY != 0)
        pos.setY(pos.y() + deltaY);
    w->move(pos);
}
/**
 * 让按钮显示颜色
 */
void setQPushButtonColor (QPushButton *btn)
{
    QPalette palette;
    palette.setColor(QPalette::Button, Qt::green); // 设置标签背景颜色为蓝色
    btn->setPalette(palette);
}
QString translate(const char * context, const char * key, const char * disambiguation, int n)
{
    return QApplication::translate(context, key, disambiguation, n);
}