#include "userMassage.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include "subUserMassageUI/friendUI.h"

userMassage::userMassage(const userManager& user): user(user) {
    stackedWidget = new QStackedWidget();
    listWidget = new QListWidget();
    auto friendList = this->user.getFriendList();

    //初始化listWidget
    //创建friend页面，并加入到stackedWidget和UIList中
    for(auto frd : friendList) {
        listWidget->addItem(frd.getUsername().c_str());

        auto frdUI = new friendUI(frd);
        friendUIList.emplace_back(frdUI);
        stackedWidget->addWidget(frdUI);
    }

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->listWidget);
    splitter->addWidget(this->stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);

    connect(listWidget,QListWidget::currentRowChanged,this,userMassage::doListWidget);

    auto mainWindow = new QVBoxLayout();
    mainWindow->addWidget(splitter);
    this->setLayout(mainWindow);
}

userMassage::~userMassage() = default;

void userMassage::doListWidget(int row) const {
    if (row >= 0 && row < friendUIList.size()) {
        stackedWidget->setCurrentIndex(row);
    }
}