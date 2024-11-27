#include "changeInfoUI.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

#include "../../mainwindow.h"
#include "../../userManager.h"

changeInfoUI::changeInfoUI(const userManager &user): user(user) {
    auto username = this->user.getUserName();
    auto usernameLabel = new QLabel("账号：");
    usernameText = new QLineEdit(username.c_str());

    this->okButton = new QPushButton("确定");

    auto usernameLine = new QHBoxLayout();
    usernameLine->addWidget(usernameLabel);
    usernameLine->addWidget(usernameText);

    auto layout = new QVBoxLayout();
    layout->addLayout(usernameLine);
    layout->addWidget(okButton);

    this->setLayout(layout);
}

changeInfoUI::~changeInfoUI() = default;
