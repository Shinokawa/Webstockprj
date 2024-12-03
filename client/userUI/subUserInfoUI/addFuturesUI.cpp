#include "addFuturesUI.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

addFuturesUI::addFuturesUI(const userManager &user): user(user) {
    okButton = new QPushButton("确定");
    okButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px 20px; border-radius: 5px;");
    
    auto newFuturesLabel = new QLabel("期货ID：");
    newFuturesLabel->setFont(QFont("Arial", 12));
    newFuturesLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    newFuturesText = new QLineEdit();
    newFuturesText->setPlaceholderText("请输入期货的InstrumentID");
    newFuturesText->setStyleSheet(
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

    auto newFuturesLine = new QHBoxLayout();
    newFuturesLine->addWidget(newFuturesLabel);
    newFuturesLine->addWidget(newFuturesText);

    auto layout = new QVBoxLayout();
    layout->setSpacing(20);
    layout->addLayout(newFuturesLine);
    layout->addWidget(okButton);
    layout->setAlignment(Qt::AlignCenter);

    setLayout(layout);
}

addFuturesUI::~addFuturesUI() = default;