#ifndef FRIENDADDUI_H
#define FRIENDADDUI_H

#include <QWidget>
#include "../../userManager.h"

class QLineEdit;
class QPushButton;

class friendAddUI : public QWidget{
    Q_OBJECT

public:
    QLineEdit* friendText;
    QPushButton* okButton;

    userManager user;

public:
    explicit friendAddUI(const userManager &user);
    ~friendAddUI() override;
};

#endif //FRIENDADDUI_H
