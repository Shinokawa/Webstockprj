#include "loginWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QResizeEvent>

loginWindow::loginWindow(QWidget *parent) : QWidget(parent) {
    // 设置背景图片
    this->pixmap = QPixmap(":/image/resources/images/bg-4.png");
    palette = QPalette();
    this->palette.setBrush(QPalette::Window, pixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 创建控件
    usernameLabel = new QLabel("账号");
    passwordLabel = new QLabel("密码");
    usernameLineEdit = new QLineEdit();
    usernameLineEdit->setPlaceholderText("请输入账号");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("请输入密码");
    loginButton = new QPushButton("登录");
    cancelButton = new QPushButton("取消");
    signupButton = new QPushButton("注册");

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
    loginButton->setStyleSheet("background-color: #5CACEE; color: white; border-radius: 5px; padding: 5px;");
    cancelButton->setStyleSheet("background-color: #FF6347; color: white; border-radius: 5px; padding: 5px;");
    signupButton->setStyleSheet("background-color: #32CD32; color: white; border-radius: 5px; padding: 5px;");

    // 创建布局
    auto loginLayout = new QGridLayout();
    loginLayout->addWidget(usernameLabel, 0, 0);
    loginLayout->addWidget(usernameLineEdit, 0, 1);
    loginLayout->addWidget(passwordLabel, 1, 0);
    loginLayout->addWidget(passwordLineEdit, 1, 1);

    // 按钮布局调整
    auto buttonHBox = new QHBoxLayout();
    buttonHBox->addWidget(loginButton, 0, Qt::AlignLeft); // 登录按钮贴左边
    buttonHBox->addStretch();                            // 添加弹性间隔
    buttonHBox->addWidget(cancelButton);                 // 居中按钮
    buttonHBox->addStretch();                            // 添加弹性间隔
    buttonHBox->addWidget(signupButton, 0, Qt::AlignRight); // 注册按钮贴右边

    // 整体布局
    auto mainLayout = new QVBoxLayout();
    mainLayout->addStretch();
    mainLayout->addLayout(loginLayout);
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

void loginWindow::updateBackground() {
    // 加载背景图片并缩放到窗口大小
    this->palette.setBrush(QPalette::Window, this->pixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    this->setPalette(palette);
}

void loginWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    int windowWidth = event->size().width();
    int fontSize = windowWidth / 40;

    QFont font("Arial", fontSize);

    usernameLabel->setFont(font);
    passwordLabel->setFont(font);
    usernameLineEdit->setFont(font);
    passwordLineEdit->setFont(font);
    loginButton->setFont(font);
    cancelButton->setFont(font);
    signupButton->setFont(font);

    updateBackground();
}

loginWindow::~loginWindow() = default;