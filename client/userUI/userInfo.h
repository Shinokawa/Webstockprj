#ifndef USERINFO_H
#define USERINFO_H

#include <QWidget>

class userManager;
class QListWidget;
class QSplitter;
class QStackedWidget;

class userInfo : public QWidget{

    Q_OBJECT

public:
    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QSplitter *splitter;

public:
    userInfo(userManager *thisUser);
    ~userInfo() override;
};

#endif //USERINFO_H
