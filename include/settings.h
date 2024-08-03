#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QFileDialog>
#include <QGraphicsView>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QColorDialog>
#include <QColor>
#include <QComboBox>
#include <QApplication>


#include <QDebug>

#include <fstream>
#include <unistd.h>
#include <iostream>

#include "utils/json.h"
#include "utils/qtui.h"
#include "utils/qtdata.h"
#include "config/version.h"
#include "utils/mfile.h"



class SystemMonitorApp;


class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
    json config_data;
    json tmp_config_data;
    json tmp;
    inline void show() { setOptions();setVisible(true);moveToCenter((QWidget*)this); }

private:
    SystemMonitorApp *m_parent;
    std::string default_path;
    std::string config_path;

    void initUI();
    void setConnect();
    QGraphicsView *view;
    QCheckBox *cbox_auto_start;
    QLabel *label_auto_start;// 开机自启动
    QCheckBox *cbox_border;
    QLabel *label_border;
    QColorDialog *q_color_dialog;  // 颜色选择器
    QPushButton *btn_color;  // 边框颜色
    QLabel *label_color;  // 边框颜色
    // 边框粗细
    // 圆角大小
    QGraphicsView *view_bkg_img;
    QLabel *label_bkg_img;  // 背景图片
    QLabel *label_bkg_img_path;  // 背景图片
    QLineEdit *lineEdit_bkg_path;
    QFileDialog *file_dialog;
    QPushButton *btn_bkg_img;
    QLabel *label_bkg_color;  // 背景色
    QLabel *label_language;  // 语言
    bool lang_flag = false;  // 修改语言标志
    QComboBox *combobox;
    QLabel *label_version;  // 版本
    QPushButton *btn_ensure;
    QPushButton *btn_cancel;
    void createOptions();
    void setStyle();
    /** 设置选项 **/
    void setOptions();
    void setBkgPath();
    void setBorderColor();
    void setBorderFlag();
    void setAutoStartFlag();
    void setLanguage();
    void setVersion();

    void update_config_json();

    void getBkgPath();
    void getBorderColor();
    void getBorderFlag();
    void getAutoStartFlag();
    void setAutoStart(bool auto_start);
    void getLanguage();
    /** 设置选项 **/
    std::string determineSystemLanguage();  // 判断(确定)系统语言
    void updateLanguage(std::string lang);
    // void initLanguage();  //  设置语言
    void cancel_setting();
    void closeEvent(QCloseEvent *event) override;
};
#endif // SETTINGS_H
