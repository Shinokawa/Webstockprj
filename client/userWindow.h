#ifndef USERWINDOW_H
#define USERWINDOW_H
#include <QWidget>
#include "userManager.h"
#include <QListWidgetItem>

class userInfo;
class userManager;
class QListWidget;
class QStackedWidget;
class QSplitter;
class userMassage;

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

public:
    explicit userWindow(const userManager& thisUser);
    ~userWindow() override;

private slots:
    void doListWidget(const QListWidgetItem *item) const;
};

#endif //USERWINDOW_H
