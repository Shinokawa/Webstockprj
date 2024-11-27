#include "userWindow.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include "userUI/userMassage.h"
#include "userUI/userInfo.h"

userWindow::userWindow(const userManager& thisUser): user(thisUser) {
    listWidget = new QListWidget();
    listWidget->addItem("我的信息");
    listWidget->addItem("我的通知");
    listWidget->addItem("我的邮箱");
    listWidget->addItem("我的期货");

    userInfo = new class userInfo(this->user);
    userMassage = new class userMassage(this->user);

    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(userInfo);
    stackedWidget->addWidget(userMassage);
    stackedWidget->setCurrentWidget(userInfo);

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->listWidget);
    splitter->addWidget(this->stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    auto *mainWindow = new QVBoxLayout();
    mainWindow->addWidget(splitter);

    connect(listWidget,&QListWidget::itemClicked, this, &userWindow::doListWidget);

    this->setLayout(mainWindow);
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
        //this->stackedWidget->setCurrentWidget();
    }

    else if(option == "我的期货") {
        //this->stackedWidget->setCurrentWidget();
    }
}
