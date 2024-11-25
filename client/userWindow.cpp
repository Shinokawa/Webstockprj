#include "userWindow.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>

#include "userUI/userInfo.h"


userWindow::userWindow(const userManager& thisUser): user(thisUser) {
    listWidget = new QListWidget();
    listWidget->addItem("我的信息");
    listWidget->addItem("我的邮箱");
    listWidget->addItem("我的期货");

    stackedWidget = new QStackedWidget();
    userInfo = new class userInfo(this->user);
    stackedWidget->addWidget(userInfo);
    stackedWidget->setCurrentWidget(userInfo);

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->listWidget);
    splitter->addWidget(this->stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    auto *mainWindow = new QVBoxLayout();
    mainWindow->addWidget(splitter);

    this->setLayout(mainWindow);
}

userWindow::~userWindow() = default;
