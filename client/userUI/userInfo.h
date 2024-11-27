#ifndef USERINFO_H
#define USERINFO_H

#include <QWidget>
#include "../userManager.h"

class userManager;
class QListWidget;
class QSplitter;
class QStackedWidget;
class QListWidgetItem;
class infoUI;
class changeInfoUI;
class changePasswordUI;

class userInfo : public QWidget{

    Q_OBJECT

public:
    userManager user;

    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QSplitter *splitter;

    infoUI *infoUI;
    changeInfoUI *changeInfoUI;
    changePasswordUI *changePasswordUI;

public:
    explicit userInfo(const userManager& thisUser);

    ~userInfo() override;

private slots:
    void doListWidget(const QListWidgetItem *item) const;

    void ci_doOKButton();   //修改信息的按钮
    void cp_doOKButton();   //修改密码的按钮
    void flashInfoUI();
};

#endif //USERINFO_H
