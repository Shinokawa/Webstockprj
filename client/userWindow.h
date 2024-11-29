#ifndef USERWINDOW_H
#define USERWINDOW_H

#include "userManager.h"
#include <QListWidgetItem>

class userInfo;
class userManager;
class QListWidget;
class QStackedWidget;
class QSplitter;
class userMassage;
class userEmail;
class userFutures;

class userWindow : public QWidget
{
    Q_OBJECT

public:
    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QSplitter *splitter;

    userManager user;

    userInfo *userInfo;
    userMassage *userMassage;
    userEmail *userEmail;
    userFutures *userFutures;

public:
    explicit userWindow(const userManager& thisUser);
    ~userWindow() override;

private slots:
    void doListWidget(const QListWidgetItem *item) const;

    void doAddFriendButton();
};

#endif //USERWINDOW_H
