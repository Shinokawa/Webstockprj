#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include "Friend.h"
#include "Email.h"
#include <vector>
#include <QJsonArray>

using namespace std;

class userManager {

private:
    string userName;
    string Password;
    string EmailAdress;
    vector<Friend> friendList;
    vector<Email> EmailList;

public:
    userManager(const string &userName, const string &password);
    ~userManager();
    string getUserName();
    string getPassword();
    string getEmailAdress();
    vector<Friend> getFriendList();
    vector<Email> getEmailList();
    void printUser() const;
    void setPassword(const string &newPassword);
    void setUsername(const string &newUsername);
    void setEmailAdress(const string &newEmailAdress);
    void initFriend();
    void initEmail();
    static QJsonArray ParseJosnToArray(const string &info, const string &title);
};

#endif //USERMANAGER_H
