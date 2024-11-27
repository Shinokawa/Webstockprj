#include "changePasswordUI.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

changePasswordUI::changePasswordUI(const userManager& user): user(user) {
    auto passwordLabel = new QLabel("原密码：");
    passwordText = new QLineEdit("请输入原密码");

    auto newPasswordLabel = new QLabel("新密码：");
    newPasswordText = new QLineEdit("请输入新密码");

    auto confirmLabel = new QLabel("确认密码：");
    confirmText = new QLineEdit("请再输入一次密码");

    okButton = new QPushButton("确定");

    auto passwordLine = new QHBoxLayout();
    passwordLine->addWidget(passwordLabel);
    passwordLine->addWidget(passwordText);

    auto newPasswordLine = new QHBoxLayout();
    newPasswordLine->addWidget(newPasswordLabel);
    newPasswordLine->addWidget(newPasswordText);

    auto confirmLine = new QHBoxLayout();
    confirmLine->addWidget(confirmLabel);
    confirmLine->addWidget(confirmText);

    auto layout = new QVBoxLayout();
    layout->addLayout(passwordLine);
    layout->addLayout(newPasswordLine);
    layout->addLayout(confirmLine);
    layout->addWidget(okButton);

    this->setLayout(layout);
}

changePasswordUI::~changePasswordUI() = default;
