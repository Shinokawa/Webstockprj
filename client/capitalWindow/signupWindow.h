#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include "loginWindow.h"
#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QPalette;
class QPixmap;

class signupWindow : public QWidget {
    Q_OBJECT

public:
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *confirmLabel;
    QLabel *EmailAdressLabel;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *confirmLineEdit;
    QLineEdit *EmailAdressEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPalette palette;
    QPixmap pixmap;

public:
    explicit signupWindow(QWidget *parent = nullptr);

    void updateBackground();

    void resizeEvent(QResizeEvent *event) override;

    ~signupWindow() override;

};

#endif //SIGNUPWINDOW_H
