#include "userManager.h"
#include <iostream>

using namespace std;

userManager::userManager(string userName, string password) {
    this->userName = userName;
    this->Password = password;
}

userManager::~userManager() {
}

string userManager::getUserName() {
    return this->userName;
}

string userManager::getPassword() {
    return this->Password;
}

void userManager::printUser() {
    cout << "用户名："<< this->userName << endl << "密码： " << this->Password << endl;
}
