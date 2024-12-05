#include "userInfo.h"
#include "../mainwindow.h"
#include <QStackedWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidgetItem>

#include "../myServer.h"
#include "subUserInfoUI/infoUI.h"
#include "subUserInfoUI/changeInfoUI.h"
#include "subUserInfoUI/changePasswordUI.h"
#include "subUserInfoUI/friendAddUI.h"
#include "subUserInfoUI/addFuturesUI.h"

userInfo::userInfo(const userManager& thisUser): user(thisUser) {
    // 初始化子页面
    infoUI = new class infoUI(user);
    changeInfoUI = new class changeInfoUI(user);
    changePasswordUI = new class changePasswordUI(user);
    friendAddUI = new class friendAddUI(user);
    addFuturesUI = new class addFuturesUI(user);

    // 左侧菜单（列表）
    listWidget = new QListWidget();
    listWidget->addItem("个人信息");
    listWidget->addItem("信息修改");
    listWidget->addItem("密码修改");
    listWidget->addItem("添加好友");
    listWidget->addItem("添加期货");

    // 美化 QListWidget
    listWidget->setStyleSheet(R"(
        QListWidget {
            background-color: #f7f9fc;
            border-right: 1px solid #d9d9d9;
            font-size: 16px;
            color: #333333;
        }
        QListWidget::item {
            padding: 10px 20px;
        }
        QListWidget::item:selected {
            background-color: #4CAF50;
            color: white;
            border-radius: 4px;
        }
    )");

    // 右侧内容区（QStackedWidget）
    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(infoUI);
    stackedWidget->addWidget(changeInfoUI);
    stackedWidget->addWidget(changePasswordUI);
    stackedWidget->addWidget(friendAddUI);
    stackedWidget->addWidget(addFuturesUI);

    stackedWidget->setCurrentWidget(infoUI);

    // 分割器（QSplitter）
    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->listWidget);
    splitter->addWidget(this->stackedWidget);

    // 设置分割器样式
    splitter->setStyleSheet(R"(
        QSplitter::handle {
            background-color: #e0e0e0;
        }
        QSplitter::handle:horizontal {
            width: 2px;
        }
    )");

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);

    // 主布局
    auto mainWindow = new QVBoxLayout();
    mainWindow->setContentsMargins(10, 10, 10, 10);
    mainWindow->addWidget(splitter);

    // 信号连接
    connect(listWidget, &QListWidget::itemClicked, this, &userInfo::doListWidget);
    connect(changePasswordUI->okButton, &QPushButton::clicked, this, &userInfo::cp_doOKButton);
    connect(changeInfoUI->okButton, &QPushButton::clicked, this, &userInfo::ci_doOKButton);

    // 应用布局
    this->setLayout(mainWindow);
    auto pixmap = QPixmap(":/image/resources/images/result.png");
    auto palette = QPalette();
    palette.setBrush(QPalette::Window, pixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

userInfo::~userInfo() = default;

void userInfo::doListWidget(const QListWidgetItem *item) const {
    string option = item->text().toStdString();

    if (option == "个人信息") {
        this->stackedWidget->setCurrentWidget(this->infoUI);
    }
    else if (option == "信息修改") {
        this->stackedWidget->setCurrentWidget(this->changeInfoUI);
    }
    else if (option == "密码修改") {
        this->stackedWidget->setCurrentWidget(this->changePasswordUI);
    }
    else if (option == "添加好友") {
        this->stackedWidget->setCurrentWidget(this->friendAddUI);
    }
    else if (option == "添加期货") {
        this->stackedWidget->setCurrentWidget(this->addFuturesUI);
    }
}

void userInfo::ci_doOKButton() {
    string newUsername = changeInfoUI->usernameText->text().toStdString();
    string newEmailAdress = changeInfoUI->EmailAdressText->text().toStdString();

    if (newUsername != user.getUserName()) {
        this->user.setUsername(newUsername);
        MainWindow::popInfoMessage("成功", "用户名成功修改成" + newUsername);
    }

    if (newEmailAdress != user.getEmailAdress()) {
        this->user.setEmailAdress(newEmailAdress);
        MainWindow::popInfoMessage("成功", "邮箱成功修改成" + newEmailAdress);
        this->user.getServer()->PostSetEmail(newEmailAdress);
    }

    flashInfoUI();
}

void userInfo::cp_doOKButton() {
    string prePassword = changePasswordUI->passwordText->text().toStdString();
    string newPassword = changePasswordUI->newPasswordText->text().toStdString();
    string confirm = changePasswordUI->confirmText->text().toStdString();
    string password = user.getPassword();

    if (prePassword == password) {
        if (newPassword == confirm) {
            user.setPassword(newPassword);
            MainWindow::popInfoMessage("成功", "密码已成功修改");
        } else {
            MainWindow::popErrorMessage("错误", "两次输入的密码不同");
        }
    } else {
        MainWindow::popErrorMessage("原密码错误", "原密码输入错误");
    }
}

void userInfo::flashInfoUI() {
    auto newUsername = "用户名：" + this->user.getUserName();
    infoUI->usernameLabel->setText(newUsername.c_str());

    auto newEmailAdress = "邮箱：" + this->user.getEmailAdress();
    infoUI->EmailLabel->setText(newEmailAdress.c_str());
}
