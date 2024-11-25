#ifndef LOGINUI_H
#define LOGINUI_H
#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QPalette;
class QPixmap;

class loginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = nullptr);

    void updateBackground();

    void resizeEvent(QResizeEvent *event) override;

    ~loginWindow() override;
public:
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QPushButton *signupButton;
    QPalette palette;
    QPixmap pixmap;
};

#endif //LOGINUI_H
