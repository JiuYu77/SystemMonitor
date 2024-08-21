#include "settings.h"
#include "systemmonitorapp.h"


SettingsDialog::SettingsDialog(QWidget *parent)
    :QDialog(parent)
{
    // m_parent = (SystemMonitorApp*)parentWidget();
    m_parent = (SystemMonitorApp*)parent;  // 上面一行也可以
    default_path = m_parent->default_path;
    config_path = m_parent->config_path;

    read_config_json(config_path, config_data);
    config_data["version"] = VERSION_ALL;  // 版本 version

    this->initUI();
    if(config_data["Language"]["flag"] == false){
        std::string lang = determineSystemLanguage();// 判断系统语言
        updateLanguage(lang);  // 根据系统语言，更新语言(更新config/config.json)
    }
    tmp_config_data = config_data;
    // this->setOptions();
    this->setConnect();
}
SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::initUI()
{
    setFixedSize(350,300);
    setWindowTitle(translate("settings", "Settings"));
    createOptions();
    setStyle();
}
void SettingsDialog::createOptions()
{
    view = new QGraphicsView(this);
    view->setObjectName("view");
    view->move(10, 10);
    view->setFixedSize(this->width()-20, this->height()-20);

    cbox_auto_start = new QCheckBox(view);
    label_auto_start = new QLabel(view);

    q_color_dialog = new QColorDialog();

    cbox_border = new QCheckBox(view);
    label_border = new QLabel(view);
    btn_color = new QPushButton(view);
    label_color = new QLabel(view);
    
    label_language = new QLabel(view);
    combobox = new QComboBox(view);


    view_bkg_img = new QGraphicsView(view);
    label_bkg_img = new QLabel(view);
    label_bkg_img_path = new QLabel(view_bkg_img);
    lineEdit_bkg_path = new QLineEdit(view_bkg_img);
    btn_bkg_img = new QPushButton(view_bkg_img);
    file_dialog = new QFileDialog();

    label_version = new QLabel(view);

    btn_ensure = new QPushButton(view);
    btn_cancel = new QPushButton(view);
    btn_cancel->setObjectName("btnCancel");
    btn_cancel->setAccessibleName("btnCancel");
}
void SettingsDialog::setStyle()
{
    const int x = 10;

    // 自启动
    moveToPos((QWidget*)cbox_auto_start, x, x);
    label_auto_start->setText(translate("settings", "Auto start"));
    label_auto_start->setFixedHeight(33);
    moveToPos((QWidget*)label_auto_start, 30, 0);

    // 边框
    cbox_border->move(x, 33);
    label_border->setText(translate("settings", "Border"));
    label_border->move(30, 30);
    btn_color->move(90, 33);
    btn_color->setFixedSize(13, 13);
    label_color->setText(translate("settings", "Border color"));
    label_color->move(110, 30);


    // 语言
    label_language->move(x, 65);
    label_language->setText(translate("settings", "Language") + ":");
    combobox->move(72, 62);
    combobox->addItem("中文(简体)");
    combobox->addItem("English");


    const int btn_width = 60;
    {
        const int btn_height = 20;
        /** 背景图片 **/
        label_bkg_img->move(x, 100);
        label_bkg_img->setText(translate("settings", "Background image"));
        view_bkg_img->setFixedSize(310, 50);
        view_bkg_img->move(x, 118);
        label_bkg_img_path->setText(translate("settings", "Path") + ":");
        label_bkg_img_path->move(5, 5);
        lineEdit_bkg_path->move(40, 5);
        lineEdit_bkg_path->setFixedSize(265, 20);

        btn_bkg_img->move(245, 28);
        btn_bkg_img->setText(translate("settings", "Browse"));
        btn_bkg_img->setFixedHeight(btn_height);
        btn_bkg_img->setFixedWidth(btn_width);
        /** 背景图片 **/
    }

    label_version->move(x, 180);
    label_version->setText(translate("settings", "Version") + ":  ");

    const int btn_height = 26;
    const int move_x = 260;
    const int move_y = 220;
    const int gap = 5;
    // 确定 取消
    btn_ensure->setText(translate("settings", "OK"));
    btn_ensure->move(move_x, move_y);
    btn_ensure->setFixedSize(btn_width, btn_height);
    btn_cancel->setText(translate("settings", "Cancel"));
    btn_cancel->move(move_x - btn_width - gap, move_y);
    btn_cancel->setFixedSize(btn_width, btn_height);
}
void SettingsDialog::setConnect()
{
    connect(btn_bkg_img, &QPushButton::clicked, this, &SettingsDialog::getBkgPath);
    connect(btn_cancel, &QPushButton::clicked, this, &SettingsDialog::cancel_setting);
    connect(btn_ensure, &QPushButton::clicked, this, &SettingsDialog::update_config_json);
    connect(btn_color, &QPushButton::clicked, this, &SettingsDialog::getBorderColor);
    connect(cbox_border, &QCheckBox::stateChanged, this, &SettingsDialog::getBorderFlag);
    connect(cbox_auto_start, &QCheckBox::stateChanged, this, &SettingsDialog::getAutoStartFlag);
    connect(combobox, QOverload<int>::of(&QComboBox::activated), this, &SettingsDialog::getLanguage);
}

/************************************************************
 *                    设置选项显示的值
 **********************************************************/
/**
 * 设置选项显示的值
 */
void SettingsDialog::setOptions()
{
    setBkgPath();
    setBorderColor();
    setBorderFlag();
    setAutoStartFlag();
    setLanguage();
    setVersion();
}
void SettingsDialog::setBkgPath()
{
    std::string bkg_img = config_data["SysAppStyle"]["bkgImage"];
    int len = bkg_img.size();
    std::string bkg_path = bkg_img.substr(22, len-24);
    if(bkg_path == ":/res/images/logo64.ico")
    {
        bkg_path = qstrToStdString(m_parent->parent_dir) + "/res/images/logo64.ico";
    }
    lineEdit_bkg_path->setText(QString::fromStdString(bkg_path));
}
/**
 * 边框颜色
 */
void SettingsDialog::setBorderColor()
{
    std::string border = config_data["SysAppStyle"]["border"];
    std::string space_delimiter = " ";
    std::string color_str;
    const size_t found = border.find_last_of(space_delimiter);
    color_str = border.substr(found+1, border.size()-found-2);

    std::cout << "color_str: ***" << color_str <<"***"<< std::endl;
    QPalette palette;
    palette.setColor(QPalette::Button, strToQstring(color_str));
    btn_color->setPalette(palette);  // 设置按钮背景颜色
}
void SettingsDialog::setBorderFlag()
{
    if(config_data["SysAppStyle"]["border_color"] == true){
        cbox_border->setChecked(true);
        return;
    }
    cbox_border->setChecked(false);
    btn_color->setEnabled(false);
}
void SettingsDialog::setAutoStartFlag()
{
    if(config_data["auto_start"] == true){
        cbox_auto_start->setChecked(true);
        return;
    }
    cbox_auto_start->setChecked(false);
}
void SettingsDialog::setLanguage()
{
    int index = config_data["Language"]["index"];
    combobox->setCurrentIndex(index);
    qDebug() <<combobox->currentText();
}
void SettingsDialog::setVersion()
{
    QString s = label_version->text();
    label_version->setText(s + strToQstring(config_data["version"]));
    // label_version->setText(QString("Version:  ") + strToQstring(config_data["version"]));
}
/************************************************************
 *                    设置选项显示的值
 **********************************************************/


/************************************************************
 *                      更新配置文件
 **********************************************************/
/**
 * 更新配置文件
 */
void SettingsDialog::update_config_json()
{
    config_data = tmp_config_data;
    writeJsonFile(config_path, config_data);
    std::cout << "update_config_json-------:" << std::endl;
    this->close();
    this->m_parent->update_UI();
    setAutoStart(config_data["auto_start"]);
    if(lang_flag){
        QMessageBox mbox;
        moveToCenter((QWidget*)&mbox);
        int ret = mbox.information(NULL,
                                    translate("settings", "Prompt"),  // 提示
                                    translate("settings", "Language modification takes effect after restarting."),  // 语言修改重启生效
                                    QMessageBox::Cancel|QMessageBox::Ok,QMessageBox::Ok);

        if(ret == QMessageBox::Cancel) // 取消修改语言
        {
            qDebug() << "QMessageBox: " << "取消修改语言" << ret ;
            tmp_config_data["Language"]["language"] = tmp["Language"]["language"];
            tmp_config_data["Language"]["index"] = tmp["Language"]["index"];
            config_data = tmp_config_data;
            writeJsonFile(config_path, config_data);
            return;
        }
        qDebug() << "QMessageBox: " << "修改语言" << ret ;
        m_parent->restart();
    }
}
void SettingsDialog::cancel_setting()
{
    qDebug() << "cancel_setting()...";
    tmp_config_data = config_data;
    this->close();
}
void SettingsDialog::getBkgPath()
{
    QString filters = "Image Files (*.ico *.png *.jpg)";
    QString qstr_path = file_dialog->getOpenFileName(NULL, "打开图片", "", filters);
    qDebug() << qstr_path;
    if(qstr_path == NULL) // 没有选择图片文件
    {
        return;
    }
    tmp_config_data["SysAppStyle"]["bkgImage"] = "background-image: url(" + qstr_path.toStdString() + ");";
    lineEdit_bkg_path->setText(qstr_path);
    qDebug() << "getBkgPath()...";
}
void SettingsDialog::getBorderColor()
{
    QColor color = q_color_dialog->getColor();
    if(!color.isValid())
    {
        qDebug() << "getBorderColor()...取消" << color;
        return;
    }
    btn_color->setPalette(color);
    qDebug() << "getBorderColor()..." << color;
    QString colorRGBA = QString("#%1").arg(color.rgba(), 8, 16, QChar('0'));
    tmp_config_data["SysAppStyle"]["border"] = "border: 2px solid " + colorRGBA.toStdString() + ";";
}
void SettingsDialog::getBorderFlag()
{
    bool border_flag = cbox_border->isChecked();
    tmp_config_data["SysAppStyle"]["border_color"] = border_flag;
    if(border_flag){
        btn_color->setEnabled(true);
    }else{
        btn_color->setEnabled(false);
    }
}
void SettingsDialog::getAutoStartFlag()
{
    bool auto_flag = cbox_auto_start->isChecked();
    tmp_config_data["auto_start"] = auto_flag;
}
void SettingsDialog::setAutoStart(bool auto_start)
{
    std::string desktop_path = qstrToStdString(m_parent->qdir_parentDir.absoluteFilePath("SystemMonitor.desktop"));
    std::cout << desktop_path<<std::endl;
    std::string home_path = getHomePath();  // 家目录
    std::string auto_dir = "/.config/autostart/";
    std::string auto_path = home_path + auto_dir + "SystemMonitor.desktop";
    std::cout << auto_path<<std::endl;

    if(auto_start == true){
        copyFile(desktop_path, auto_path);
        qDebug() << "copy SystemMonitor.desktop...";
    }else if(auto_start == false){
        removeFile(auto_path);
    }
}
void SettingsDialog::getLanguage()
{
    tmp = config_data;
    QString language_qstr = combobox->currentText();
    int index = combobox->currentIndex();
    if(tmp_config_data["Language"]["index"] == index){
        lang_flag = false;
        qDebug() << "getLanguage()..."<<lang_flag;
        return;
    }
    tmp_config_data["Language"]["language"] = qstrToStdString(language_qstr);
    tmp_config_data["Language"]["index"] = index;
    qDebug() << "getLanguage()..."<<language_qstr;
    lang_flag = true;  // true: 点击确定按钮后，重启并修改语言
    qDebug() << "getLanguage()..."<<lang_flag;
}
void SettingsDialog::closeEvent(QCloseEvent *event)
{
    cancel_setting();
    event->accept();
    qDebug() << "closeEvent()...";
}

/************************************************************
 *                      更新配置文件
 **********************************************************/
std::string SettingsDialog::determineSystemLanguage()
{
    const char* lang = std::getenv("LANG");
    std::string ret;
    if (lang) {
        std::cout << "System language is: " << lang << std::endl;
        ret = lang;
    } else { // lang == NULL
        std::cout << "System language not set." << std::endl;
        ret = "NULL";
    }
    return ret;
}
void SettingsDialog::updateLanguage(std::string lang)
{
    std::cout << "updateLanguage()..." << std::endl;

    size_t idx = lang.find("zh");
    int index = 0;
    if(idx != std::string::npos){
        config_data["Language"]["language"] = qstrToStdString(combobox->itemText(index));
        config_data["Language"]["index"] = index;
    }else if(lang == "NULL" || idx == std::string::npos){
        // English
        index = 1;
        config_data["Language"]["language"] = qstrToStdString(combobox->itemText(index));
        config_data["Language"]["index"] = index;
    }
    config_data["Language"]["flag"] = true;
    writeJsonFile(config_path, config_data);  // 更新config/config.json
}