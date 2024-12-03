#include "changeInfoUI.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

changeInfoUI::changeInfoUI(const userManager &user): user(user) {
    auto username = this->user.getUserName();
    auto usernameLabel = new QLabel("账号：");
    usernameLabel->setFont(QFont("Arial", 12));
    usernameLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    usernameText = new QLineEdit(username.c_str());
    usernameText->setStyleSheet(
        "QLineEdit {"
        "    background-color: #ffffff;"
        "    border: 2px solid #4CAF50;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "    font-size: 14px;"
        "    color: #333333;"
        "}"
        "QLineEdit:focus {"
        "    border-color: #3e8e41;"
        "}"
    );

    auto EmailAdress = this->user.getEmailAdress();
    auto EmailAdressLabel = new QLabel("邮箱：");
    EmailAdressLabel->setFont(QFont("Arial", 12));
    EmailAdressLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    EmailAdressText = new QLineEdit(EmailAdress.c_str());
    EmailAdressText->setStyleSheet(
        "QLineEdit {"
        "    background-color: #ffffff;"
        "    border: 2px solid #4CAF50;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "    font-size: 14px;"
        "    color: #333333;"
        "}"
        "QLineEdit:focus {"
        "    border-color: #3e8e41;"
        "}"
    );

    this->okButton = new QPushButton("确定");
    okButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px 20px; border-radius: 5px;");

    auto usernameLine = new QHBoxLayout();
    usernameLine->addWidget(usernameLabel);
    usernameLine->addWidget(usernameText);

    auto EmailAdressLine = new QHBoxLayout();
    EmailAdressLine->addWidget(EmailAdressLabel);
    EmailAdressLine->addWidget(EmailAdressText);

    auto layout = new QVBoxLayout();
    layout->setSpacing(20);
    layout->addLayout(usernameLine);
    layout->addLayout(EmailAdressLine);
    layout->addWidget(okButton);
    layout->setAlignment(Qt::AlignCenter);

    auto palette = QPalette(this->palette());
    palette.setColor(QPalette::Window,QColor(245,199,246));
    this->setPalette(palette);
    this->setLayout(layout);
}

changeInfoUI::~changeInfoUI() = default;