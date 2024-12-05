#include "mainwindow.h"
#include "capitalWindow/loginWindow.h"
#include "capitalWindow/signupWindow.h"
#include <QPushButton>
#include <QStackedWidget>
#include <string>
#include <QLineEdit>
#include <QMessageBox>
#include "dataManager.h"
#include "userManager.h"
#include "capitalWindow/userWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    dataManager = new class dataManager();

    loginWindow = new class loginWindow();
    signupWindow = new class signupWindow();
    userWindow = nullptr;

    this->stackWidget = new QStackedWidget();
    this->stackWidget->addWidget(loginWindow);
    this->stackWidget->addWidget(signupWindow);

    setCentralWidget(this->stackWidget);

    connect(loginWindow->signupButton,&QPushButton::clicked,this,&MainWindow::doSignupButton);
    connect(loginWindow->loginButton,&QPushButton::clicked,this,&MainWindow::doLoginButton);
    connect(loginWindow->cancelButton,&QPushButton::clicked,this,&MainWindow::doLogCancelButton);
    connect(signupWindow->okButton,&QPushButton::clicked,this,&MainWindow::doOKButton);
    connect(signupWindow->cancelButton,&QPushButton::clicked,this,&MainWindow::doSignCancelButton);

}

MainWindow::~MainWindow() = default;

void MainWindow::popErrorMessage(const string& title, const string& message) {
    QMessageBox::critical(nullptr, tr(title.c_str()),  tr(message.c_str()),
                                   QMessageBox::Ok);
}

void MainWindow::popInfoMessage(const string& title, const string& message) {
    QMessageBox::information(nullptr,tr(title.c_str()),  tr(message.c_str()),
                                   QMessageBox::Ok);
}

void MainWindow::doSignupButton() const {
    this->stackWidget->setCurrentWidget(signupWindow);
}

void MainWindow::doLoginButton() {
    string inputUserName = loginWindow->usernameLineEdit->text().toStdString();
    string inputPassword = loginWindow->passwordLineEdit->text().toStdString();

    bool isFind = false;

    for(userManager user : dataManager->getUserList()){
        if(user.getUserName() == inputUserName) {
            isFind = true;
            if(user.getPassword() == inputPassword) {
                this->userWindow = new class userWindow(user);
                this->stackWidget->addWidget(userWindow);
                this->stackWidget->setCurrentWidget(userWindow);
            }
            else {
                popErrorMessage("密码错误", "请检查密码是否正确");
            }
        }
    }
    if(!isFind) {
        popErrorMessage("用户不存在", "请检查账号是否正确");
    }
}

void MainWindow::doLogCancelButton() {
    close();
}

void MainWindow::doOKButton() const {    //注册事件
    const string inputUserName = signupWindow->usernameLineEdit->text().toStdString();
    const string inputPassword = signupWindow->passwordLineEdit->text().toStdString();
    const string inputConfirm = signupWindow->confirmLineEdit->text().toStdString();
    const string inputEmailAdress = signupWindow->EmailAdressEdit->text().toStdString();

    if(inputConfirm == inputPassword) {
        const auto newUser = userManager(inputUserName, inputPassword, inputEmailAdress);
        dataManager->addUser(newUser);

        loginWindow->usernameLineEdit->setText(QString::fromStdString(inputUserName));
        loginWindow->passwordLineEdit->setText(QString::fromStdString(inputPassword));

        this->dataManager->printUserList();

        this->stackWidget->setCurrentWidget(this->loginWindow);
    }
    else {
        popErrorMessage("输入错误", "输入的两次密码不同");
    }
}

void MainWindow::doSignCancelButton() const {
    this->stackWidget->setCurrentWidget(this->loginWindow);
}


