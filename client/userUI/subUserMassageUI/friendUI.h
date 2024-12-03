#ifndef FRIENDUI_H
#define FRIENDUI_H

#include <QWidget>
#include "../../userData/Friend.h"
#include "../../userManager.h"

class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QLabel;

class friendUI : public QWidget{

public:
    userManager user;
    Friend frd;

    QTextEdit *sendBox;
    QPushButton *sendButton;
    QVBoxLayout *dialogueLayout;

public:
    explicit friendUI(const userManager &user, const Friend& frd);
    ~friendUI() override;
    void fromJsonLineToLabel(const QJsonValue &value) const;

private slots:
    void doSendButton() const;
};

#endif //FRIENDUI_H
