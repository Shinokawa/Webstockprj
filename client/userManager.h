#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include "Friend.h"
#include <vector>

using namespace std;

class userManager {

private:
    string userName;
    string Password;
    vector<Friend> frends;

public:
    userManager(const string &userName, const string &password);
    ~userManager();
    string getUserName();
    string getPassword();
    vector<Friend> getFriendList();

    void printUser() const;
    void setPassword(const string &newPassword);

    void setUsername(const string &newUsername);

    void initFriend();
};

#endif //USERMANAGER_H
