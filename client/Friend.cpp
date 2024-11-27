#include "Friend.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "userManager.h"

Friend::Friend(const string& friendInfo) {
    /*
    QString jsonString = R"({
        "friendName": "我的客服",
        "dialogue": [
            {"sendName": "我的客服", "sendMessage": "测试"},
            {"sendName": "我", "sendMessage": "测试测试"}
        ]
    })";
    */
    auto jsonString = QString::fromStdString(friendInfo);
    // 将 JSON 字符串转换为 QByteArray，因为 QJsonDocument 的解析器需要 QByteArray
    QByteArray jsonData = jsonString.toUtf8();

    // 将 JSON 数据解析为 QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 获取 JSON 对象
    messageJson = jsonDoc.object();
    username = messageJson["friendName"].toString().toStdString();
}

Friend::~Friend() = default;

QJsonObject Friend::getMessage() {
    return this->messageJson;
}

string Friend::getUsername() {
    return this->username;
}

void Friend::flashMessage(const QJsonObject& newMessageJson) {
    /*
    QString jsonString = R"({
        "friendName": "我的客服",
        "dialogue": [
            {"sendName": "我的客服", "sendMessage": "测试"},
            {"sendName": "我", "sendMessage": "测试测试"}
        ]
    })";
    */
    // 获取 dialogue 数组
    QJsonArray dialogueArray = messageJson["dialogue"].toArray();

    dialogueArray.append(messageJson);

    messageJson["dialogue"] = dialogueArray;
}
