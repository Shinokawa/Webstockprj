#include "userManager.h"
#include <iostream>
#include <string>
#include "myServer.h"
#include "Friend.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

using namespace std;

userManager::userManager(const string &userName, const string &password) {
    this->userName = userName;
    this->Password = password;
    this->EmailAdress = "无";
    initFriend();
    initEmail();
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

    auto friendArray = ParseJosnToArray(friendsInfo, "friend");

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

    auto EmailArray = ParseJosnToArray(EmailInfo, "Email");

    for(auto EmailJson : EmailArray) {
        auto Eml = Email(EmailJson.toObject());
        EmailList.emplace_back(Eml);
    }
}

QJsonArray userManager::ParseJosnToArray(const string &info, const string &title) {
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
