#ifndef INFOUI_H
#define INFOUI_H

#include <QWidget>

class userManager;
class QLabel;

class infoUI : public QWidget {

    Q_OBJECT

public:
    QLabel *usernameLabel;
    QLabel *EmailLabel;

public:
    explicit infoUI(userManager user);
    ~infoUI() override;
};

#endif //INFOUI_H
