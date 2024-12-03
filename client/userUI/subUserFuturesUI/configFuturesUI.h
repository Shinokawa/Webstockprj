#ifndef CONFIGFUTURESUI_H
#define CONFIGFUTURESUI_H

#include <QWidget>
#include "../../userData/Futures.h"

class QPushButton;
class QLineEdit;

class configFuturesUI : public QWidget{

public:
    Futures Ftrs;
    QPushButton *okButton;
    QLineEdit *highPriceWarningText;
    QLineEdit *lowPriceWarningText;

public:
    explicit configFuturesUI(const Futures& Ftrs);
    ~configFuturesUI() override;
};



#endif //CONFIGFUTURESUI_H
