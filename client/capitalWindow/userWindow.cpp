#include "userWindow.h"

#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include "../mainwindow.h"
#include "../userUI/userFutures.h"
#include "../userUI/userEmail.h"
#include "../userUI/userMassage.h"
#include "../userUI/userInfo.h"
#include "../userUI/subUserInfoUI/friendAddUI.h"

userWindow::userWindow(const userManager& thisUser): user(thisUser) {
    // 初始化左侧菜单栏
    listWidget = new QListWidget();
    listWidget->addItem("我的信息");
    listWidget->addItem("我的通知");
    listWidget->addItem("我的邮箱");
    listWidget->addItem("我的期货");

    // 美化左侧菜单栏
    listWidget->setStyleSheet(R"(
        QListWidget {
            background-color: #f5f5f5;
            border: none;
            font-size: 14px;
        }
        QListWidget::item {
            padding: 12px 20px;
            border-bottom: 1px solid #ddd;
            color: #555;
        }
        QListWidget::item:hover {
            background-color: #e0e0e0;
        }
        QListWidget::item:selected {
            background-color: #0078D7;
            color: white;
        }
    )");

    // 创建并初始化子页面
    userInfo = new class userInfo(this->user);
    userMassage = new class userMassage(this->user);
    userEmail = new class userEmail(this->user);
    userFutures = new class userFutures(this->user);

    // 设置stackedWidget
    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(userInfo);
    stackedWidget->addWidget(userMassage);
    stackedWidget->addWidget(userEmail);
    stackedWidget->addWidget(userFutures);

    stackedWidget->setCurrentWidget(userInfo);

    // 美化QSplitter
    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(listWidget);
    splitter->addWidget(stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    splitter->setStyleSheet(R"(
        QSplitter::handle {
            background-color: #d1d1d1;
            width: 3px;
        }
        QSplitter::handle:hover {
            background-color: #0078D7;
        }
    )");

    // 布局设置
    auto *mainWindow = new QVBoxLayout();
    mainWindow->setContentsMargins(10, 10, 10, 10);  // 设置整体边距
    mainWindow->addWidget(splitter);

    // 设置页面的布局
    this->setLayout(mainWindow);

    // 信号与槽连接
    connect(listWidget, &QListWidget::itemClicked, this, &userWindow::doListWidget);
    connect(userInfo->friendAddUI->okButton,&QPushButton::clicked,this,&userWindow::doAddFriendButton);
}

userWindow::~userWindow() = default;

void userWindow::doListWidget(const QListWidgetItem *item) const {
    string option = item->text().toStdString();

    if(option == "我的信息") {
        this->stackedWidget->setCurrentWidget(this->userInfo);
    }

    else if(option == "我的通知") {
        this->stackedWidget->setCurrentWidget(this->userMassage);
    }

    else if(option == "我的邮箱") {
        this->stackedWidget->setCurrentWidget(this->userEmail);
    }

    else if(option == "我的期货") {
        this->stackedWidget->setCurrentWidget(this->userFutures);
    }
}

void userWindow::doAddFriendButton() {
    string friendName = userInfo->friendAddUI->friendText->text().toStdString();
    auto frd = user.addNewFriend(friendName);
    userMassage->flashFriendList(frd);
    MainWindow::popInfoMessage("成功","好友添加成功");
}