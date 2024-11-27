#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

using namespace std;

class loginWindow;
class signupWindow;
class userWindow;
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
    userWindow *userWindow;
    QStackedWidget *stackWidget;
    dataManager *dataManager;

public:
    static void popErrorMessage(const std::string& title, const std::string& message);

    static void popInfoMessage(const string &title, const string &message);

private slots:
    void doSignupButton() const;
    void doLoginButton();

    void doLogCancelButton();

    void doOKButton() const;
    void doSignCancelButton() const;
};
#endif // MAINWINDOW_H