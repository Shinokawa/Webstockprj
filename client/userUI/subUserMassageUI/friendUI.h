#ifndef FRIENDUI_H
#define FRIENDUI_H

#include <QWidget>
#include "../../userData/Friend.h"

class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QLabel;

class friendUI : public QWidget{

public:
    Friend frd;

    QTextEdit *sendBox;
    QPushButton *sendButton;
    QVBoxLayout *dialogueLayout;

public:
    explicit friendUI(const Friend& frd);
    ~friendUI() override;
    void fromJsonLineToLabel(const QJsonValue &value) const;

private slots:
    void doSendButton();
};

#endif //FRIENDUI_H
