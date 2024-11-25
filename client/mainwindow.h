#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

using namespace std;

class loginWindow;
class signupWindow;
class QVBoxLayout;
class dataManager;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    loginWindow *loginWindow;
    signupWindow *signupWindow;
    QStackedWidget *stackWidget;
    dataManager *dataManager;

private slots:
    void doSignupButton() const;
    void doLoginButton();

    void popErrorMessage(const std::string& title, const std::string& message);

    void doLogCancelButton();

    void doOKButton();
    void doSignCancelButton() const;
};
#endif // MAINWINDOW_H
