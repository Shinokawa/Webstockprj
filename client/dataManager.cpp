#include "dataManager.h"
#include "userManager.h"

dataManager::dataManager() {
    auto user = userManager("123","123","1664044536@qq.com");
    this->userList.emplace_back(user);
}

dataManager::~dataManager() = default;

vector<userManager> dataManager::getUserList() {
    return this->userList;
}

void dataManager::addUser(const userManager& newUser) {
    this->userList.emplace_back(newUser);
}

void dataManager::printUserList() {
    for(const userManager& user : this->userList) {
        user.printUser();
    }
}
