#ifndef ADDFUTURESUI_H
#define ADDFUTURESUI_H
#include <QWidget>
#include "../../userManager.h"

class QPushButton;
class QLineEdit;

class addFuturesUI : public QWidget{

    Q_OBJECT

public:
    userManager user;
    QPushButton *okButton;
    QLineEdit *newFuturesText;
    QLineEdit *highPriceText;
    QLineEdit *lowPriceText;
    QLineEdit *AlertTimeText;

public:
    explicit addFuturesUI(const userManager &user);
    ~addFuturesUI() override;

};

#endif //ADDFUTURESUI_H
