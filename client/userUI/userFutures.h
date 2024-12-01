#ifndef USERFUTURES_H
#define USERFUTURES_H

#include <QWidget>
#include "../userManager.h"

class QLabel;
class QListWidget;
class QStackedWidget;
class QVBoxLayout;
class FuturesUI;

class userFutures : public QWidget{

public:
    userManager user;
    vector<FuturesUI*> FuturesUIList;

    QListWidget *listWidget;
    QWidget *optionWidget;
    QStackedWidget *stackedWidget;
    QStackedWidget *allStackedWidget;
    QVBoxLayout *mainWindow;

public:
    explicit userFutures(const userManager& user);
    ~userFutures() override;

private slots:
    void doListWidget(int row) const;
    void doExitButton() const;
    void doDeleteButton(const FuturesUI* FtrsUI);
};

#endif //USERFUTURES_H
