#ifndef USERWINDOW_H
#define USERWINDOW_H

#include "../userManager.h"
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

    void fetchIsAlert() const;  //异步获取是否警报

private slots:
    void doListWidget(const QListWidgetItem *item) const;

    void doAddFriendButton();
    void doAddFuturesButton();
    void updateAlertInfo() const;   //更新数据
};

#endif //USERWINDOW_H
