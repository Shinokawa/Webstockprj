#ifndef USERFUTURES_H
#define USERFUTURES_H

#include <QWidget>
#include "../userManager.h"

class QLabel;
class QListWidget;
class QStackedWidget;
class QVBoxLayout;
class FuturesUI;
class QListWidgetItem;

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

    static QListWidgetItem* initListItem(const Futures &Ftrs);
    void flashFuturesList(const Futures &newFutures);

    void fetchFuturesData() const;  //异步获取期货信息

private slots:
    void doListWidget(int row) const;
    void doExitButton() const;
    void doDeleteButton(const FuturesUI* FtrsUI) const;
    void updateFuturesUI(const string &data) const;   //更新UI的方法
};

#endif //USERFUTURES_H
