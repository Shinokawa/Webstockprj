#include "userManager.h"
#include <iostream>
#include <string>
#include "myServer.h"
#include "Friend.h"

using namespace std;

userManager::userManager(const string &userName, const string &password) {
    this->userName = userName;
    this->Password = password;
    this->EmailAdress = "无";
    initFriend();
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
    return this->frends;
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
    /*  最终是这样的
    string friendInfo;
    while((friendInfo = myServer.ReqFriendInfo(userName)) != nullptr) {
        auto friend = Friend(friendInfo);
        frends.emplace_back(friend);
    }
    */
    string friendInfo1 = R"({
        "friendName": "我的客服",
        "dialogue": [
            {"sendName": "我的客服", "sendMessage": "测试"},
            {"sendName": "我", "sendMessage": "测试测试"}
        ]
    })";

    string friendInfo2 = R"({
        "friendName": "KaSumi",
        "dialogue": [
            {"sendName": "KaSumi", "sendMessage": "KiRaKiRa~"},
            {"sendName": "我", "sendMessage": "DoKiDoki~"}
        ]
    })";

    auto frd1 = Friend(friendInfo1);
    frends.emplace_back(frd1);

    auto frd2 = Friend(friendInfo2);
    frends.emplace_back(frd2);
}
