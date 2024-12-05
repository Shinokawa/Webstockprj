#include "userWindow.h"

#include <QJsonDocument>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QSplitter>
#include <QTimer>
#include <QVBoxLayout>
#include <QtConcurrent/qtconcurrentrun.h>

#include "../mainwindow.h"
#include "../userUI/userFutures.h"
#include "../userUI/userEmail.h"
#include "../userUI/userMassage.h"
#include "../userUI/userInfo.h"
#include "../userUI/subUserFuturesUI/FuturesUI.h"
#include "../userUI/subUserInfoUI/addFuturesUI.h"
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
    connect(userInfo->addFuturesUI->okButton,&QPushButton::clicked,this,&userWindow::doAddFuturesButton);

    //异步
    // 启动定时器，每5秒钟获取一次聊天数据
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &userWindow::fetchIsAlert);
    timer->start(1000);  // 每1000毫秒（即1秒）执行一次 fetchIsAlert

    // 异步调用，使用lambda表达式来包装成员函数
    // ReSharper disable once CppNoDiscardExpression
    QtConcurrent::run([this] {
        this->fetchIsAlert();  // 在后台线程调用fetchIsAlert
    });
}

userWindow::~userWindow() = default;

void userWindow::fetchIsAlert() const {
    emit(updateAlertInfo());
}

void userWindow::updateAlertInfo() const {

    for (auto ftrsUI : userFutures->FuturesUIList) {
        double price = ftrsUI->Ftrs.LastPrice;
        double highPrice = ftrsUI->Ftrs.highPriceWarning;
        double lowPrice = ftrsUI->Ftrs.lowPriceWarning;
        QString instrumentID = QString::fromStdString(ftrsUI->Ftrs.InstrumentID);

        if (price > highPrice || price < lowPrice) {
            QString message = "价格不在限定值里，请到客服处查看详情";
            QString title = "警告";

            // 使用 QTimer::singleShot 将 popInfoMessage 调度到主线程
            QTimer::singleShot(0, [title, message]() {
                MainWindow::popInfoMessage(title.toStdString(), message.toStdString());
            });

            QString data;
            if (price > highPrice) {
                data = QString(R"({
                "sendName": "我的客服",
                "sendMessage": "%1"
            })").arg(instrumentID + "的价格超过了设定的最高价格");
                ftrsUI->Ftrs.highPriceWarning = INT64_MAX;
            }
            else {
                data = QString(R"({
                "sendName": "我的客服",
                "sendMessage": "%1"
            })").arg(instrumentID + "的价格低于了设定的最低价格");
                ftrsUI->Ftrs.lowPriceWarning = -1;
            }

            QJsonObject jsonObj = QJsonDocument::fromJson(data.toUtf8()).object();
            userMassage->updateOfficialChatData(jsonObj);
        }

    }
}

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

void userWindow::doAddFuturesButton() {
    string InstrumentID = userInfo->addFuturesUI->newFuturesText->text().toStdString();
    double highPrice = userInfo->addFuturesUI->highPriceText->text().toDouble();
    double lowPrice = userInfo->addFuturesUI->lowPriceText->text().toDouble();
    string alertTime = userInfo->addFuturesUI->AlertTimeText->text().toStdString();
    auto newFutures = user.addNewFutures(InstrumentID, highPrice, lowPrice, alertTime);
    userFutures->flashFuturesList(newFutures);
    MainWindow::popInfoMessage("成功",InstrumentID + "添加成功");
}