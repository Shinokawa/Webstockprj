#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include "userData/Friend.h"
#include "userData/Email.h"
#include "userData/Futures.h"
#include <vector>
#include <QJsonArray>

using namespace std;

class myServer;

class userManager {

public:
    string testFu;

private:
    string userName;
    string Password;
    string EmailAdress;
    vector<Friend> friendList;
    vector<Email> EmailList;
    vector<Futures> FuturesList;

    myServer *server;

public:
    userManager(const string &userName, const string &password);
    ~userManager();
    string getUserName();
    string getPassword();
    string getEmailAdress();
    vector<Friend> getFriendList();
    vector<Email> getEmailList();
    vector<Futures> getFuturesList();
    myServer* getServer() const;
    void printUser() const;
    void setPassword(const string &newPassword);
    void setUsername(const string &newUsername);
    void setEmailAdress(const string &newEmailAdress);
    void initFriend();
    void initEmail();
    void initFutures();
    Friend addNewFriend(const string& friendName);
    Futures addNewFutures(const string& InstrumentID);
    static QJsonArray ParseJosnToArray(const string &info, const string &title);
};

#endif //USERMANAGER_H
