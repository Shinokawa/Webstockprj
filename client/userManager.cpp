#include "userManager.h"
#include <iostream>

using namespace std;

userManager::userManager(const string &userName, const string &password) {
    this->userName = userName;
    this->Password = password;
}

userManager::~userManager() = default;

string userManager::getUserName() {
    return this->userName;
}

string userManager::getPassword() {
    return this->Password;
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
