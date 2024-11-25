#ifndef CHANGEPASSWORDUI_H
#define CHANGEPASSWORDUI_H

#include <QWidget>
#include "../../userManager.h"

class userManager;
class QLineEdit;
class QPushButton;

class changePasswordUI : public QWidget {

    Q_OBJECT

public:
    QLineEdit* passwordText;
    QLineEdit* newPasswordText;
    QLineEdit* confirmText;
    QPushButton* okButton;
    userManager user;

public:
    explicit changePasswordUI(const userManager& user);
    ~changePasswordUI() override;
};

#endif // CHANGEPASSWORDUI_H
