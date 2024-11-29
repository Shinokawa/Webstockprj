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

    // 初始化 listWidget
    for (auto frd : friendList) {
        listWidget->addItem(frd.getUsername().c_str());

        auto frdUI = new friendUI(frd);
        friendUIList.emplace_back(frdUI);
        stackedWidget->addWidget(frdUI);
    }

    // 美化 listWidget
    listWidget->setStyleSheet(R"(
        QListWidget {
            background-color: #f8f9fa;
            border: none;
        }
        QListWidget::item {
            padding: 10px;
            border-bottom: 1px solid #e1e1e1;
        }
        QListWidget::item:hover {
            background-color: #e9ecef;
        }
        QListWidget::item:selected {
            background-color: #0078D7;
            color: white;
        }
    )");

    // 美化 QSplitter
    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(listWidget);
    splitter->addWidget(stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);
    splitter->setStyleSheet(R"(
        QSplitter::handle {
            background-color: #d1d1d1;
            width: 2px;
        }
        QSplitter::handle:hover {
            background-color: #0078D7;
        }
    )");

    // 美化 stackedWidget
    stackedWidget->setStyleSheet("background-color: #ffffff; border-radius: 5px;");

    // 布局调整
    auto mainWindow = new QVBoxLayout();
    mainWindow->setContentsMargins(10, 10, 10, 10);  // 设置边距
    mainWindow->addWidget(splitter);

    this->setLayout(mainWindow);

    // 连接信号槽
    connect(listWidget, &QListWidget::currentRowChanged, this, &userMassage::doListWidget);
}

userMassage::~userMassage() = default;

void userMassage::doListWidget(int row) const {
    if (row >= 0 && row < friendUIList.size()) {
        stackedWidget->setCurrentIndex(row);
    }
}

void userMassage::flashFriendList(Friend frd) {
    this->listWidget->addItem(frd.getUsername().c_str());
    auto frdUI = new friendUI(frd);
    friendUIList.emplace_back(frdUI);
    stackedWidget->addWidget(frdUI);
}