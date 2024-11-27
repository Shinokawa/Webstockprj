#ifndef CHANGEINFOUI_H
#define CHANGEINFOUI_H

#include <QWidget>
#include "../../userManager.h"

class QLineEdit;
class QPushButton;

class changeInfoUI : public QWidget {

    Q_OBJECT

public:
    QLineEdit* usernameText;
    QPushButton* okButton;

    userManager user;

public:
    explicit changeInfoUI(const userManager &user);
    ~changeInfoUI() override;
};

#endif //CHANGEINFOUI_H
