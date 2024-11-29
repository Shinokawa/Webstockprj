#include "Friend.h"
#include <QJsonObject>
#include <QJsonValue>
#include "../userManager.h"

Friend::Friend(const QJsonObject& friendInfo) {
    /*
    QString jsonString = R"({
        "friendName": "我的客服",
        "dialogue": [
            {"sendName": "我的客服", "sendMessage": "测试"},
            {"sendName": "我", "sendMessage": "测试测试"}
        ]
    })";
    */
    // 获取 JSON 对象
    messageJson = friendInfo;
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
