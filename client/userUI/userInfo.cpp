#include "userInfo.h"
#include "../mainwindow.h"
#include <QStackedWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidgetItem>
#include "subUserInfoUI/infoUI.h"
#include "subUserInfoUI/changeInfoUI.h"
#include "subUserInfoUI/changePasswordUI.h"

userInfo::userInfo(const userManager& thisUser): user(thisUser) {
    infoUI = new class infoUI(user);
    changeInfoUI = new class changeInfoUI(user);
    changePasswordUI = new class changePasswordUI(user);

    listWidget = new QListWidget();
    listWidget->addItem("个人信息");
    listWidget->addItem("信息修改");
    listWidget->addItem("密码修改");

    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(infoUI);
    stackedWidget->addWidget(changeInfoUI);
    stackedWidget->addWidget(changePasswordUI);
    stackedWidget->setCurrentWidget(infoUI);

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->listWidget);
    splitter->addWidget(this->stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);

    auto mainWindow = new QVBoxLayout();
    mainWindow->addWidget(splitter);

    connect(listWidget, &QListWidget::itemClicked, this, &userInfo::doListWidget);
    connect(changePasswordUI->okButton, &QPushButton::clicked, this, &userInfo::cp_doOKButton);
    connect(changeInfoUI->okButton, &QPushButton::clicked, this, &userInfo::ci_doOKButton);

    this->setLayout(mainWindow);
}

userInfo::~userInfo() = default;

void userInfo::doListWidget(const QListWidgetItem *item) const {
    string option = item->text().toStdString();

    if(option == "个人信息") {
        this->stackedWidget->setCurrentWidget(this->infoUI);
    }

    else if(option == "信息修改") {
        this->stackedWidget->setCurrentWidget(this->changeInfoUI);
    }

    else if(option == "密码修改") {
        this->stackedWidget->setCurrentWidget(this->changePasswordUI);
    }
}

void userInfo::ci_doOKButton() {
    string newUsername = changeInfoUI->usernameText->text().toStdString();
    this->user.setUsername(newUsername);
    MainWindow::popInfoMessage("成功","用户名成功修改成" + newUsername);

    flashInfoUI();
}

void userInfo::cp_doOKButton() {
    string prePassword = changePasswordUI->passwordText->text().toStdString();
    string newPassword = changePasswordUI->newPasswordText->text().toStdString();
    string confirm = changePasswordUI->confirmText->text().toStdString();
    string password = user.getPassword();

    if(prePassword == password) {
        if(newPassword == confirm) {
            user.setPassword(newPassword);
            MainWindow::popInfoMessage("成功","密码已成功修改");
        }
        else {
            MainWindow::popErrorMessage("错误","两次输入的密码不同");
        }
    }
    else {
        MainWindow::popErrorMessage("原密码错误","原密码输入错误");
    }
}

void userInfo::flashInfoUI() {
    auto username = this->user.getUserName();
    auto information = "用户名：" + username;
    infoUI->usernameLabel->setText(information.c_str());
}
