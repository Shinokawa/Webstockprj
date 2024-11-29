#include "friendAddUI.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

friendAddUI::friendAddUI(const userManager &user): user(user) {
    // 设置界面标题或主标签
    auto friendLabel = new QLabel("好友用户名：");
    friendLabel->setFont(QFont("Arial", 14, QFont::Bold));  // 设置字体样式
    friendLabel->setStyleSheet("color: #4a4a4a; font-size: 16px; padding: 5px;");

    // 输入框
    friendText = new QLineEdit();
    friendText->setPlaceholderText("请输入用户名");
    friendText->setFont(QFont("Arial", 12));
    friendText->setStyleSheet(
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

    // 确定按钮
    this->okButton = new QPushButton("确定");
    okButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 5px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #388e3c;"
        "    padding: 12px 22px;"
        "}"
    );

    // 布局
    auto friendLine = new QHBoxLayout();
    friendLine->addWidget(friendLabel);
    friendLine->addWidget(friendText);
    friendLine->setSpacing(10);  // 控件之间的间距

    auto layout = new QVBoxLayout();
    layout->setSpacing(25);  // 控件之间的间距
    layout->addLayout(friendLine);
    layout->addWidget(okButton);
    layout->setAlignment(Qt::AlignCenter);

    // 设置窗口的外观
    this->setLayout(layout);
}

friendAddUI::~friendAddUI() = default;

