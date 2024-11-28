#ifndef EMAILUI_H
#define EMAILUI_H
#include <QWidget>
#include "../../Email.h"

class QPushButton;

class EmailUI : public QWidget{

    Q_OBJECT

public:
    QPushButton *exitButton;

public:
    Email Eml;

public:
    explicit EmailUI(const Email &Eml);
    ~EmailUI() override;
};

#endif //EMAILUI_H
