#ifndef USERWINDOW_H
#define USERWINDOW_H
#include <QWidget>

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

public:
    explicit  userWindow(userManager* thisUser);
    ~userWindow() override;
};

#endif //USERWINDOW_H
