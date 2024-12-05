#include "signupWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QResizeEvent>

signupWindow::signupWindow(QWidget *parent) : QWidget(parent) {
    // 设置背景图片
    this->pixmap = QPixmap(":/image/resources/images/bg-4.png");
    palette = QPalette();
    this->palette.setBrush(QPalette::Window, pixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 创建控件
    usernameLabel = new QLabel("账号");
    passwordLabel = new QLabel("密码");
    confirmLabel = new QLabel("确认密码");
    EmailAdressLabel = new QLabel("邮箱");
    usernameLineEdit = new QLineEdit();
    usernameLineEdit->setPlaceholderText("请输入账号");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("请输入密码");
    confirmLineEdit = new QLineEdit();
    confirmLineEdit->setEchoMode(QLineEdit::Password);
    confirmLineEdit->setPlaceholderText("请确认您的密码");
    EmailAdressEdit = new QLineEdit();
    EmailAdressEdit->setPlaceholderText("请输入你的邮箱地址");
    okButton = new QPushButton("确定");
    cancelButton = new QPushButton("取消");

    // 设置控件样式
    usernameLineEdit->setStyleSheet(R"(
    QLineEdit {
        background: rgba(255, 255, 255, 0.7);
        border-radius: 5px;
        padding-left: 30px;
        background-image: url(:/image/resources/images/user-icon.png);
        background-repeat: no-repeat;
        background-position: left center;
        background-size: 20px 20px;
    })");
    passwordLineEdit->setStyleSheet(R"(
    QLineEdit {
        background: rgba(255, 255, 255, 0.7);
        border-radius: 5px;
        padding-left: 30px;
        background-image: url(:/image/resources/images/password-icon.png);
        background-repeat: no-repeat;
        background-position: left center;
        background-size: 20px 20px;
    })");
    confirmLineEdit->setStyleSheet(R"(
    QLineEdit {
        background: rgba(255, 255, 255, 0.7);
        border-radius: 5px;
        padding-left: 30px;
        background-image: url(:/image/resources/images/password-icon.png);
        background-repeat: no-repeat;
        background-position: left center;
        background-size: 20px 20px;
    })");
    EmailAdressEdit->setStyleSheet(R"(
    QLineEdit {
        background: rgba(255, 255, 255, 0.7);
        border-radius: 5px;
        padding-left: 30px;
        background-image: url(:/image/resources/images/user-icon.png);
        background-repeat: no-repeat;
        background-position: left center;
        background-size: 20px 20px;
    })");
    okButton->setStyleSheet("background-color: #5CACEE; color: white; border-radius: 5px; padding: 5px;");
    cancelButton->setStyleSheet("background-color: #FF6347; color: white; border-radius: 5px; padding: 5px;");

    usernameLabel->setStyleSheet("color: blue; font-size: 18px;");
    passwordLabel->setStyleSheet("color: blue; font-size: 18px;");
    confirmLabel->setStyleSheet("color: blue; font-size: 18px;");
    EmailAdressLabel->setStyleSheet("color: blue; font-size: 18px;");

    // 布局
    auto signupLayout = new QGridLayout();
    signupLayout->addWidget(usernameLabel, 0, 0);
    signupLayout->addWidget(usernameLineEdit, 0, 1);

    signupLayout->addWidget(passwordLabel, 1, 0);
    signupLayout->addWidget(passwordLineEdit, 1, 1);

    signupLayout->addWidget(confirmLabel, 2, 0);
    signupLayout->addWidget(confirmLineEdit, 2, 1);

    signupLayout->addWidget(EmailAdressLabel,3,0);
    signupLayout->addWidget(EmailAdressEdit,3,1);

    // 按钮布局
    auto buttonHBox = new QHBoxLayout();
    buttonHBox->addWidget(okButton, 0, Qt::AlignLeft); // 确定按钮贴左
    buttonHBox->addStretch();
    buttonHBox->addWidget(cancelButton, 0, Qt::AlignRight); // 取消按钮贴右

    // 主布局
    auto mainLayout = new QVBoxLayout();
    mainLayout->addStretch();
    mainLayout->addLayout(signupLayout);
    mainLayout->addLayout(buttonHBox);
    mainLayout->addStretch();

    // 添加透明背景面板
    auto container = new QWidget();
    container->setLayout(mainLayout);
    container->setStyleSheet("background: rgba(255, 255, 255, 0.8); border-radius: 10px;");

    auto containerLayout = new QVBoxLayout(this);
    containerLayout->addStretch();
    containerLayout->addWidget(container, 0, Qt::AlignCenter);
    containerLayout->addStretch();

    this->setLayout(containerLayout);
}

void signupWindow::updateBackground() {
    // 加载背景图片并缩放到窗口大小
    this->palette.setBrush(QPalette::Window, this->pixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    this->setPalette(palette);
}

void signupWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // 窗口大小变化时更新背景
    updateBackground();

    // 根据窗口大小动态调整字体大小
    int windowWidth = event->size().width();
    int fontSize = windowWidth / 40; // 根据窗口宽度设置字体大小比例
    QFont font("Arial", fontSize);

    usernameLabel->setFont(font);
    passwordLabel->setFont(font);
    confirmLabel->setFont(font);
    EmailAdressLabel->setFont(font);
    usernameLineEdit->setFont(font);
    passwordLineEdit->setFont(font);
    confirmLineEdit->setFont(font);
    EmailAdressEdit->setFont(font);
    okButton->setFont(font);
    cancelButton->setFont(font);
}

signupWindow::~signupWindow() = default;

