#include "dataManager.h"

#include "userManager.h"

dataManager::dataManager() {
    this->userList.emplace_back("123","123");
}

dataManager::~dataManager() = default;

vector<userManager> dataManager::getUserList() {
    return this->userList;
}

void dataManager::addUser(const userManager& newUser) {
    this->userList.emplace_back(newUser);
}

void dataManager::printUserList() {
    for(userManager user : this->userList) {
        user.printUser();
    }
}
