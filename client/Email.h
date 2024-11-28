#ifndef EMAIL_H
#define EMAIL_H

#include <QJsonObject>
#include <string>

using namespace std;

class Email {

private:
    string From;
    string To;
    string Subject;
    string Body;

public:
    explicit Email(const QJsonObject &EmialInfo);
    ~Email();
    string getFrom();
    string getTo();
    string getSubject();
    string getBody();
};



#endif //EMAIL_H
