#ifndef CONFIGFUTURESUI_H
#define CONFIGFUTURESUI_H

#include <QWidget>
#include "../../userData/Futures.h"

class QPushButton;

class configFuturesUI : public QWidget{

public:
    Futures Ftrs;
    QPushButton *okButton;

public:
    configFuturesUI(const Futures& Ftrs);
    ~configFuturesUI() override;
};



#endif //CONFIGFUTURESUI_H
