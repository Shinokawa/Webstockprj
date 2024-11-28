#ifndef USEREMAIL_H
#define USEREMAIL_H

#include <QWidget>
#include <vector>
#include "../userManager.h"

using namespace std;

class QListWidget;
class QStackedWidget;
class QVBoxLayout;
class EmailUI;

class userEmail : public QWidget{

    Q_OBJECT

public:
    userManager user;

    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QStackedWidget *allStackedWidget;
    QVBoxLayout *mainWindow;
    vector<EmailUI*> EmailUIList;

public:
    explicit userEmail(const userManager& user);
    ~userEmail() override;

    private slots:
        void doListWidget(int row) const;
        void doExitButton() const;
};

#endif //USEREMAIL_H
