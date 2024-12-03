#include "Email.h"
#include "../myServer.h"

Email::Email(const QJsonObject &EmailInfo) {
    /*
    {
        "From": "anon1664044536@163.com",
        "To" : "1664044536@qq.com",
        "Subject" : "紧急通知1",
        "Body" : "我让你加仓你二龙嘛？"
    }
    */
    this->From = EmailInfo["From"].toString().toStdString();
    this->To = EmailInfo["To"].toString().toStdString();
    this->Subject = EmailInfo["Subject"].toString().toStdString();
    this->Body = EmailInfo["Body"].toString().toStdString();
}

Email::~Email() = default;

string Email::getFrom() {
    return this->From;
}

string Email::getTo() {
    return this->To;
}

string Email::getSubject() {
    return this->Subject;
}

string Email::getBody() {
    return this->Body;
}
