#ifndef MYSERVER_H
#define MYSERVER_H

#include <QtNetwork/QNetworkRequest>
#include <string>
#include "userManager.h"
#include "userData/Email.h"
#include "userData/Friend.h"
#include <QCoreApplication>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>

using namespace std;

class myServer : public QObject{

    Q_OBJECT

private:
    userManager user;
    QNetworkAccessManager *m_manager;

public:
    explicit myServer(const userManager &user);
    string PostFromURL(const string &url, const string &data);
    string GetFromURL(const string &url);
    string GetChatData();
    void PostMessage(const QJsonObject &data);
};

#endif //MYSERVER_H
