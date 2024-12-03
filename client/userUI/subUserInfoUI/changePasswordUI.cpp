#include "changePasswordUI.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>

changePasswordUI::changePasswordUI(const userManager& user): user(user) {
    auto passwordLabel = new QLabel("原密码：");
    passwordLabel->setFont(QFont("Arial", 12));
    passwordLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    passwordText = new QLineEdit();
    passwordText->setPlaceholderText("请输入原密码");  // 设置密码框提示文字
    passwordText->setEchoMode(QLineEdit::Password);  // 密码框隐藏输入
    passwordText->setStyleSheet(
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

    auto newPasswordLabel = new QLabel("新密码：");
    newPasswordLabel->setFont(QFont("Arial", 12));
    newPasswordLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    newPasswordText = new QLineEdit();
    newPasswordText->setPlaceholderText("请输入新密码");
    newPasswordText->setEchoMode(QLineEdit::Password);
    newPasswordText->setStyleSheet(
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

    auto confirmLabel = new QLabel("确认密码：");
    confirmLabel->setFont(QFont("Arial", 12));
    confirmLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    confirmText = new QLineEdit();
    confirmText->setPlaceholderText("请再输入一次密码");
    confirmText->setEchoMode(QLineEdit::Password);
    confirmText->setStyleSheet(
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

    okButton = new QPushButton("确定");
    okButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px 20px; border-radius: 5px;");

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
    layout->setSpacing(20);
    layout->addLayout(passwordLine);
    layout->addLayout(newPasswordLine);
    layout->addLayout(confirmLine);
    layout->addWidget(okButton);
    layout->setAlignment(Qt::AlignCenter);

    this->setLayout(layout);
}

changePasswordUI::~changePasswordUI() = default;
