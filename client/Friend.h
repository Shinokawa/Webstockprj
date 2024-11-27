#ifndef FRIEND_H
#define FRIEND_H

#include <string>
#include <QJsonObject>

using namespace std;

class Friend {

private:
    string username;
    QJsonObject messageJson;

public:
    explicit Friend(const string& friendInfo);
    ~Friend();
    QJsonObject getMessage();
    string getUsername();
    void flashMessage(const QJsonObject& newMessageJson);
};



#endif //FRIEND_H
