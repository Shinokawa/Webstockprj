#ifndef USERWINDOW_H
#define USERWINDOW_H
#include <QWidget>
#include "userManager.h"

class userInfo;
class userManager;
class QListWidget;
class QStackedWidget;
class QSplitter;

class userWindow : public QWidget
{
    Q_OBJECT

public:
    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QSplitter *splitter;

    userManager user;

    userInfo *userInfo;

public:
    explicit userWindow(const userManager& thisUser);
    ~userWindow() override;
};

#endif //USERWINDOW_H
