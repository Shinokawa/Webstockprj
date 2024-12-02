#include "userManager.h"
#include <iostream>
#include <string>
#include "myServer.h"
#include "userData/Friend.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

using namespace std;

userManager::userManager(const string &userName, const string &password){
    this->userName = userName;
    this->Password = password;
    this->EmailAdress = "无";
    initFriend();
    initEmail();
    initFutures();
    this->server = new myServer(*this);
}

userManager::~userManager() = default;

string userManager::getUserName() {
    return this->userName;
}

string userManager::getPassword() {
    return this->Password;
}

string userManager::getEmailAdress() {
    return this->EmailAdress;
}

void userManager::setEmailAdress(const string &newEmailAdress) {
    this->EmailAdress = newEmailAdress;
}

vector<Friend> userManager::getFriendList() {
    return this->friendList;
}

vector<Email> userManager::getEmailList() {
    return this->EmailList;
}

vector<Futures> userManager::getFuturesList() {
    return this->FuturesList;
}

myServer* userManager::getServer() const {
    return this->server;
}

void userManager::printUser() const {
    cout << "用户名："<< this->userName << endl << "密码： " << this->Password << endl;
}

void userManager::setPassword(const string &newPassword) {
    this->Password = newPassword;
}

void userManager::setUsername(const string &newUsername) {
    this->userName = newUsername;
}

void userManager::initFriend() {
    string friendsInfo = R"({
    "friend": [
        {
            "friendName": "我的客服",
            "dialogue": [
                {"sendName": "我的客服", "sendMessage": "测试"},
                {"sendName": "我", "sendMessage": "测试测试"}
            ]
        },
        {
            "friendName": "KaSumi",
            "dialogue": [
                {"sendName": "KaSumi", "sendMessage": "KiRaKiRa~"},
                {"sendName": "我", "sendMessage": "DoKiDoki~"}
            ]
        }
    ]
    })";

    auto friendArray = ParseJsonToArray(friendsInfo, "friend");

    for(auto frdJson : friendArray) {
        auto frd = Friend(frdJson.toObject());
        friendList.emplace_back(frd);
    }
}

void userManager::initEmail() {
    string EmailInfo = R"({
    "Email": [
        {
            "From": "anon1664044536@163.com",
            "To" : "1664044536@qq.com",
            "Subject" : "紧急通知1",
            "Body" : "我让你加仓你二龙嘛？"
        },
        {
            "From": "anon1664044536@outlook.com",
            "To" : "1664044536@qq.com",
            "Subject" : "紧急通知2",
            "Body" : "我让你加仓了吗？"
        }
    ]
    })";
    auto EmailArray = ParseJsonToArray(EmailInfo, "Email");

    for(auto EmailJson : EmailArray) {
        auto Eml = Email(EmailJson.toObject());
        EmailList.emplace_back(Eml);
    }
}

void userManager::initFutures() {
    string FuturesInfo = R"({
    "Futures": [
        {
          "TradingDay": 20241130,
          "reserve1": "some_value",
          "ExchangeID": "SHFE",
          "reserve2": "some_value",
          "LastPrice": 3400.50,
          "PreSettlementPrice": 3395.80,
          "PreClosePrice": 3390.20,
          "PreOpenInterest": 100000,
          "OpenPrice": 3405.00,
          "HighestPrice": 3420.00,
          "LowestPrice": 3380.00,
          "Volume": 50000,
          "Turnover": 170000000,
          "OpenInterest": 120000,
          "ClosePrice": 3410.00,
          "SettlementPrice": 3405.50,
          "UpperLimitPrice": 3450.00,
          "LowerLimitPrice": 3350.00,
          "PreDelta": 0.05,
          "CurrDelta": 0.02,
          "UpdateTime": 144500,
          "UpdateMillisec": 250,
          "BidPrice1": 3400.00,
          "BidVolume1": 100,
          "AskPrice1": 3401.00,
          "AskVolume1": 150,
          "BidPrice2": 3399.00,
          "BidVolume2": 120,
          "AskPrice2": 3402.00,
          "AskVolume2": 130,
          "BidPrice3": 3398.00,
          "BidVolume3": 90,
          "AskPrice3": 3403.00,
          "AskVolume3": 110,
          "BidPrice4": 3397.00,
          "BidVolume4": 80,
          "AskPrice4": 3404.00,
          "AskVolume4": 100,
          "BidPrice5": 3396.00,
          "BidVolume5": 70,
          "AskPrice5": 3405.00,
          "AskVolume5": 90,
          "AveragePrice": 3403.50,
          "ActionDay": 20241130,
          "InstrumentID": "M1901",
          "ExchangeInstID": "M1901.SHFE",
          "BandingUpperPrice": 3500.00,
          "BandingLowerPrice": 3300.00,
          "lowPriceWarning": 1500,
          "highPriceWarning": 1600
        },
        {
          "TradingDay": 20241201,
          "reserve1": "some_value_2",
          "ExchangeID": "CME",
          "reserve2": "some_value_2",
          "LastPrice": 1561.30,
          "PreSettlementPrice": 2550.00,
          "PreClosePrice": 2542.20,
          "PreOpenInterest": 150000,
          "OpenPrice": 2548.10,
          "HighestPrice": 2570.00,
          "LowestPrice": 2530.00,
          "Volume": 75000,
          "Turnover": 190000000,
          "OpenInterest": 135000,
          "ClosePrice": 2560.50,
          "SettlementPrice": 2552.00,
          "UpperLimitPrice": 2600.00,
          "LowerLimitPrice": 2500.00,
          "PreDelta": 0.03,
          "CurrDelta": 0.04,
          "UpdateTime": 150000,
          "UpdateMillisec": 300,
          "BidPrice1": 2545.00,
          "BidVolume1": 200,
          "AskPrice1": 2546.50,
          "AskVolume1": 250,
          "BidPrice2": 2544.00,
          "BidVolume2": 180,
          "AskPrice2": 2547.00,
          "AskVolume2": 230,
          "BidPrice3": 2543.50,
          "BidVolume3": 150,
          "AskPrice3": 2548.00,
          "AskVolume3": 200,
          "BidPrice4": 2543.00,
          "BidVolume4": 130,
          "AskPrice4": 2549.00,
          "AskVolume4": 170,
          "BidPrice5": 2542.50,
          "BidVolume5": 110,
          "AskPrice5": 2550.00,
          "AskVolume5": 160,
          "AveragePrice": 2547.00,
          "ActionDay": 20241201,
          "InstrumentID": "ESZ20",
          "ExchangeInstID": "ESZ20.CME",
          "BandingUpperPrice": 2650.00,
          "BandingLowerPrice": 2450.00,
          "lowPriceWarning": 2000,
          "highPriceWarning": 3000
    }
    ]
    })";
    //string url = "http://www.shakouzu.top:8786/marketdata/m2501";
    //auto data = server->GetFromURL(url);
    //testFu = data;
    auto FuturesArray = ParseJsonToArray( FuturesInfo, "Futures");

    for(auto FuturesJson : FuturesArray) {
        auto Ftrs = Futures(FuturesJson.toObject());
        FuturesList.emplace_back(Ftrs);
    }

}

Friend userManager::addNewFriend(const string& friendName) {
    QString jsonString = QString(R"({
    "friendName": "%1",
    "dialogue": [
        {"sendName": "我", "sendMessage": "你好"}
    ]
    })").arg(QString::fromStdString(friendName));

    auto jsonData = jsonString.toUtf8();
    auto jsonDoc = QJsonDocument::fromJson(jsonData);
    auto newFriendJson = jsonDoc.object();

    auto newFriend = Friend(newFriendJson);

    friendList.emplace_back(newFriend);
    return newFriend;
}

Futures userManager::addNewFutures(const string &InstrumentID) {
    QString FuturesInfo = QString(R"({
        "TradingDay": 20241201,
        "reserve1": "new_value",
        "ExchangeID": "DCE",
        "reserve2": "new_value",
        "LastPrice": 4500.75,
        "PreSettlementPrice": 4490.50,
        "PreClosePrice": 4485.00,
        "PreOpenInterest": 120000,
        "OpenPrice": 4502.00,
        "HighestPrice": 4520.00,
        "LowestPrice": 4480.00,
        "Volume": 60000,
        "Turnover": 270000000,
        "OpenInterest": 140000,
        "ClosePrice": 4510.00,
        "SettlementPrice": 4508.50,
        "UpperLimitPrice": 4550.00,
        "LowerLimitPrice": 4450.00,
        "PreDelta": 0.04,
        "CurrDelta": 0.03,
        "UpdateTime": 145000,
        "UpdateMillisec": 500,
        "BidPrice1": 4500.00,
        "BidVolume1": 150,
        "AskPrice1": 4501.00,
        "AskVolume1": 200,
        "BidPrice2": 4499.00,
        "BidVolume2": 130,
        "AskPrice2": 4502.00,
        "AskVolume2": 180,
        "BidPrice3": 4498.00,
        "BidVolume3": 100,
        "AskPrice3": 4503.00,
        "AskVolume3": 150,
        "BidPrice4": 4497.00,
        "BidVolume4": 90,
        "AskPrice4": 4504.00,
        "AskVolume4": 120,
        "BidPrice5": 4496.00,
        "BidVolume5": 80,
        "AskPrice5": 4505.00,
        "AskVolume5": 100,
        "AveragePrice": 4504.25,
        "ActionDay": 20241201,
        "InstrumentID": "%1",
        "ExchangeInstID": "M1902.DCE",
        "BandingUpperPrice": 4600.00,
        "BandingLowerPrice": 4400.00,
        "lowPriceWarning": 2000,
        "highPriceWarning": 2100
    })").arg(QString::fromStdString(InstrumentID));

    auto jsonData = FuturesInfo.toUtf8();
    auto jsonDoc = QJsonDocument::fromJson(jsonData);

    auto newFuturesJson = jsonDoc.object();
    auto newFutures = Futures(newFuturesJson);
    FuturesList.emplace_back(newFutures);
    return newFutures;
}

QJsonArray userManager::ParseJsonToArray(const string &info, const string &title) {
    auto jsonString = QString::fromStdString(info);
    // 将 JSON 字符串转换为 QByteArray，因为 QJsonDocument 的解析器需要 QByteArray

    auto jsonData = jsonString.toUtf8();

    // 将 JSON 数据解析为 QJsonDocument
    auto jsonDoc = QJsonDocument::fromJson(jsonData);

    // 获取 JSON 对象
    auto messageJson = jsonDoc.object();

    // 获取 friend 数组
    auto Array = messageJson[title.c_str()].toArray();
    return Array;
}
