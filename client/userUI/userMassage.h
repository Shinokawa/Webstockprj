#ifndef USERMASSAGE_H
#define USERMASSAGE_H

#include <QWidget>
#include <vector>
#include "../userManager.h"

using namespace std;

class QListWidget;
class QStackedWidget;
class QSplitter;
class friendUI;

class userMassage : public QWidget{

    Q_OBJECT

public:
    userManager user;

    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QSplitter *splitter;

    vector<friendUI*> friendUIList;

public:
    explicit userMassage(const userManager& user);
    ~userMassage() override;void
    flashFriendList(Friend frd);

    void updateOfficialChatData(const QJsonObject &dataJson);

    void fetchChatData();   //异步获取服务器聊天数据

private slots:
    void doListWidget(int row) const;

    static void updateFriendChatData(const friendUI* frdUI, const QJsonObject& dataJson); //异步的槽函数
};
#endif //USERMASSAGE_H
