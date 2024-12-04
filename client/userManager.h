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

private:
    string userName;
    string Password;
    string EmailAdress;
    vector<Friend> friendList;
    vector<Email> EmailList;
    vector<Futures> FuturesList;
    vector<string> starInfoList;

    myServer *server;

public:
    userManager(const string &userName, const string &password, const string &EmailAdress);
    ~userManager();
    string getUserName();
    string getPassword();
    string getEmailAdress();
    vector<Friend> getFriendList();
    vector<Email> getEmailList();
    vector<Futures> getFuturesList();
    [[nodiscard]] myServer* getServer() const;
    void printUser() const;
    void setPassword(const string &newPassword);
    void setUsername(const string &newUsername);
    void setEmailAdress(const string &newEmailAdress);
    void initFriend();
    void initEmail();
    Friend addNewFriend(const string& friendName);
    Futures addNewFutures(const string &InstrumentID, const double &highPriceWarning, const double &lowPriceWarning);
    static QJsonArray ParseJsonToArray(const string &info, const string &title);

    void flashStarInfo(const string &starInfo);

    void flashFutures(const QJsonObject &FuturesItem);

    string FromVectorStringToJsonString();
};

#endif //USERMANAGER_H
