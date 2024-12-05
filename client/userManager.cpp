#include "userManager.h"
#include <iostream>
#include <string>
#include "myServer.h"
#include "userData/Friend.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <qthread.h>

using namespace std;

userManager::userManager(const string &userName, const string &password, const string &EmailAdress){
    this->userName = userName;
    this->Password = password;
    this->EmailAdress = EmailAdress;
    this->server = new myServer(*this);
    initFriend();
    initEmail();
    string starInfo = R"({
        "starInfo": [
            {
              "InstrumentID": "m2501",
              "lowPriceWarning": 1500,
              "highPriceWarning": 1600,
              "alertTime": "24:00:00"
            },
            {
              "InstrumentID": "m2505",
              "lowPriceWarning": 2000,
              "highPriceWarning": 3000,
              "alertTime": "24:00:00"
    	    }
    ]})";
    flashStarInfo(starInfo); //包含了对期货的初始化
    server->PostSetEmail(EmailAdress);
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
                {"sendName": "我的客服", "sendMessage": "你好"}
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
            "Subject" : "感谢您的注册",
            "Body" : "感谢你使用本Webstock的软件"
        }
    ]
    })";
    auto EmailArray = ParseJsonToArray(EmailInfo, "Email");

    for(auto EmailJson : EmailArray) {
        auto Eml = Email(EmailJson.toObject());
        EmailList.emplace_back(Eml);
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

Futures userManager::addNewFutures(const string &InstrumentID, const double &highPriceWarning, const double &lowPriceWarning, const string &alertTime) {
    QString starInfo = QString(R"({
            "InstrumentID": "%1",
            "lowPriceWarning": %2,
            "highPriceWarning": %3,
            "alertTime": "%4"
        })").arg(QString::fromStdString(InstrumentID)).arg(lowPriceWarning).arg(highPriceWarning).arg(QString::fromStdString(alertTime));
    starInfoList.emplace_back(starInfo.toStdString());
    auto newInfo = FromVectorStringToJsonString();
    flashStarInfo(newInfo);

    for (const auto& futures : FuturesList) {
        if (futures.InstrumentID == InstrumentID) {
            return futures;
        }
    }

    throw runtime_error("Futures not found");
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

void userManager::flashStarInfo(const string &starInfo) {
    auto starArray = ParseJsonToArray(starInfo,"starInfo");

    QJsonObject FuturesJson;
    QJsonArray FuturesArray;
    auto newStarList = vector<string>();
    for (auto starJson : starArray) {
        QJsonArray instrumentsArray;

        auto InstrumentID = starJson.toObject()["InstrumentID"].toString();
        double lowPriceWarning = starJson.toObject()["lowPriceWarning"].toDouble();
        double highPriceWarning = starJson.toObject()["highPriceWarning"].toDouble();
        auto alertTime = starJson.toObject()["alertTime"].toString();

        QString FuturesItem = QString(R"({
            "InstrumentID": "%1",
            "lowPriceWarning": %2,
            "highPriceWarning": %3,
            "alertTime": "%4"
        })").arg(InstrumentID).arg(lowPriceWarning).arg(highPriceWarning).arg(alertTime);
        // 将 FuturesItem 字符串解析为 JSON 对象
        QJsonDocument doc = QJsonDocument::fromJson(FuturesItem.toUtf8());
        QJsonObject futuresItemObj = doc.object();  // 转换为 QJsonObject
        // 将 JSON 对象添加到 QJsonArray 中
        FuturesArray.append(futuresItemObj);
        instrumentsArray.append(InstrumentID);
        newStarList.emplace_back(FuturesItem.toStdString());

        QJsonObject postDataObject;
        postDataObject["uuid"] = "user-1234";
        postDataObject["instruments"] = instrumentsArray;
        postDataObject["time_alert"] = alertTime;
        postDataObject["resistance_price"] = highPriceWarning;
        postDataObject["support_price"] = lowPriceWarning;

        server->PostStarInfo(postDataObject);   //更新订阅
    }
    starInfoList = newStarList;
    FuturesJson["Futures"] = FuturesArray;

    flashFutures(FuturesJson);
}

void userManager::flashFutures(const QJsonObject &FuturesItem) {
    QThread::msleep(2000);
    // 获取 FuturesInfo 数据
    auto FuturesInfo = server->GetStarInfo();

    // 解析 FuturesInfo 和 FuturesItem
    QJsonDocument doc1 = QJsonDocument::fromJson(QString::fromStdString(FuturesInfo).toUtf8());
    QJsonArray futuresArray1 = doc1.array();  // 获取 FuturesInfo 中的数组

    // 解析 FuturesItem 数据
    QJsonDocument doc2(FuturesItem);
    QJsonArray futuresArray2 = doc2.object()["Futures"].toArray();  // 获取 FuturesItem 中的 Futures 数组

    // 遍历 FuturesItem，将对应的高低价警告信息合并到 FuturesInfo 中
    for (auto && i : futuresArray2) {
        QJsonObject item2 = i.toObject();
        QString instrumentID = item2["InstrumentID"].toString();

        // 在 FuturesInfo 中找到对应的 InstrumentID
        for (auto && j : futuresArray1) {
            QJsonObject item1 = j.toObject();
            if (item1["InstrumentID"].toString() == instrumentID) {
                // 合并高低价警告信息
                item1["highPriceWarning"] = item2["highPriceWarning"];
                item1["lowPriceWarning"] = item2["lowPriceWarning"];
                item1["alertTime"] = item2["alertTime"];
                j = item1;  // 更新 FuturesInfo 中的对象
                break;
            }
        }
    }

    // 创建一个新的 QJsonObject 来存放合并后的数据
    QJsonObject mergedFuturesJson;
    mergedFuturesJson["Futures"] = futuresArray1;  // 更新后的 Futures 数组

    // 将合并后的 QJsonObject 转换为 JSON 字符串
    QJsonDocument mergedDoc(mergedFuturesJson);
    QString mergedJsonString = mergedDoc.toJson(QJsonDocument::Compact);

    auto FuturesArray = ParseJsonToArray(mergedJsonString.toStdString(), "Futures");

    auto newFuturesList = vector<Futures>();
    for(auto FuturesJson : FuturesArray) {
        QJsonObject FuturesObj = FuturesJson.toObject();

        if (!FuturesObj.isEmpty()) {
            newFuturesList.emplace_back(FuturesObj);
        }
    }
    FuturesList = newFuturesList;
}

string userManager::FromVectorStringToJsonString() {
    QJsonArray starInfoArray;

    // 遍历 starInfoList 中的每个 JSON 字符串
    for (const auto& item : starInfoList) {
        // 将每个字符串转换为 QJsonObject
        QString jsonStr = QString::fromStdString(item);
        QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
        if (doc.isObject()) {
            QJsonObject jsonObj = doc.object();
            // 将该对象添加到 starInfoArray 中
            starInfoArray.append(jsonObj);
        }
    }

    // 构造最终的 JSON 对象，包含 "starInfo" 数组
    QJsonObject finalObject;
    finalObject["starInfo"] = starInfoArray;

    // 转换为 JSON 字符串
    QJsonDocument finalDoc(finalObject);
    QString finalJsonStr = finalDoc.toJson(QJsonDocument::Compact);

    return finalJsonStr.toStdString();  // 返回最终的 JSON 字符串
}