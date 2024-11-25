#include "userWindow.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include "userUI/userInfo.h"

userWindow::userWindow(userManager *thisUser) {
    listWidget = new QListWidget();
    listWidget->addItem("我的信息");
    listWidget->addItem("我的邮箱");
    listWidget->addItem("我的邮箱");
    listWidget->addItem("我的期货");

    stackedWidget = new QStackedWidget();
    auto userInfo = new class userInfo(thisUser);

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->listWidget);
    splitter->addWidget(this->stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

}

userWindow::~userWindow() = default;
