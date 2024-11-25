#include "userInfo.h"
#include "../userManager.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>

userInfo::userInfo(userManager *thisUser) {
    listWidget = new QListWidget();
    listWidget->addItem("个人信息");
    listWidget->addItem("信息修改");
    listWidget->addItem("密码修改");

    stackedWidget = new QStackedWidget();

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->listWidget);
    splitter->addWidget(this->stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
}

userInfo::~userInfo() = default;
