#include "userMassage.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>
#include <QJsonArray>
#include "../myServer.h"
#include "subUserMassageUI/friendUI.h"

userMassage::userMassage(const userManager& user): user(user) {
    stackedWidget = new QStackedWidget();
    listWidget = new QListWidget();
    auto friendList = this->user.getFriendList();

    // 初始化 listWidget
    for (auto frd : friendList) {
        listWidget->addItem(frd.getUsername().c_str());

        auto frdUI = new friendUI(this->user, frd);
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

    //异步
    // 启动定时器，每5秒钟获取一次聊天数据
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &userMassage::fetchChatData);
    timer->start(10000);  // 每3000毫秒（即3秒）执行一次 fetchChatData

    // 异步调用，使用lambda表达式来包装成员函数
    // ReSharper disable once CppNoDiscardExpression
    QtConcurrent::run([this] {
        this->fetchChatData();  // 在后台线程调用fetchFuturesData
    });
}

userMassage::~userMassage() = default;

void userMassage::doListWidget(int row) const {
    if (row >= 0 && row < friendUIList.size()) {
        stackedWidget->setCurrentIndex(row);
    }
}

void userMassage::flashFriendList(Friend frd) {
    this->listWidget->addItem(frd.getUsername().c_str());
    auto frdUI = new friendUI(user, frd);
    friendUIList.emplace_back(frdUI);
    stackedWidget->addWidget(frdUI);
}

void userMassage::fetchChatData() {
    //string data = user.getServer()->GetChatData();
    string data = R"({
    "friend": [
        {
            "friendName": "我的客服",
            "dialogue": [
                {"sendName": "我的客服", "sendMessage": "new"},
                {"sendName": "我", "sendMessage": "new"}
            ]
        },
        {
            "friendName": "KaSumi",
            "dialogue": [
                {"sendName": "KaSumi", "sendMessage": "new"},
                {"sendName": "我", "sendMessage": "new"}
            ]
        }
    ]
    })";
    auto friendArray = userManager::ParseJsonToArray(data,"friend");   //把string变成jsonArray
    for(auto frdMsg : friendArray) {        //遍历每一个好友的对话
        auto frdJson = frdMsg.toObject();         //把对话的JsonValueRef变成JsonObject
        auto frdName = frdJson["friendName"].toString().toStdString();
        for(auto frdUI : friendUIList) {          //匹配名称
            if(frdName == frdUI->frd.getUsername()) {
                auto dialogueJson = frdJson["dialogue"].toArray();      //把好友对话的json变成jsonArray
                for(auto dialogueLine : dialogueJson) {      //遍历jsonArray，对每一行进行添加
                    emit updateFriendChatData(frdUI,dialogueLine.toObject());
                }
            }
        }
    }
}

void userMassage::updateFriendChatData(const friendUI* frdUI, const QJsonObject& dataJson) {
    frdUI->frd.flashMessage(dataJson);   //添加到好友的对话List里 
    frdUI->fromJsonLineToLabel(dataJson);
}

void userMassage::updateOfficialChatData(const QJsonObject& dataJson) {
    for (auto frdUI : friendUIList) {
        if (frdUI->frd.getUsername() == "我的客服") {
            frdUI->fromJsonLineToLabel(dataJson);
        }
    }
}