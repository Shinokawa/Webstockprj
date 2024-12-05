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

    auto lowPriceLabel = new QLabel("最低价格：");
    lowPriceLabel->setFont(QFont("Arial", 12));
    lowPriceLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    auto highPriceLabel = new QLabel("最高价格：");
    highPriceLabel->setFont(QFont("Arial", 12));
    highPriceLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    auto  AlertTimeLabel = new QLabel("时间限制：");
    AlertTimeLabel->setFont(QFont("Arial", 12));
    AlertTimeLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

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

    lowPriceText = new QLineEdit();
    lowPriceText->setPlaceholderText("请输入期货的最低价格");
    lowPriceText->setStyleSheet(
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

    highPriceText = new QLineEdit();
    highPriceText->setPlaceholderText("请输入期货的最高价格");
    highPriceText->setStyleSheet(
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

    AlertTimeText = new QLineEdit();
    AlertTimeText->setPlaceholderText("请输入期货的预警时间");
    AlertTimeText->setStyleSheet(
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

    auto highPriceLine = new QHBoxLayout();
    highPriceLine->addWidget(highPriceLabel);
    highPriceLine->addWidget(highPriceText);

    auto lowPriceLine = new QHBoxLayout();
    lowPriceLine->addWidget(lowPriceLabel);
    lowPriceLine->addWidget(lowPriceText);

    auto AlertTimeLine = new QHBoxLayout();
    AlertTimeLine->addWidget(AlertTimeLabel);
    AlertTimeLine->addWidget(AlertTimeText);

    auto layout = new QVBoxLayout();
    layout->setSpacing(20);
    layout->addLayout(newFuturesLine);
    layout->addLayout(highPriceLine);
    layout->addLayout(lowPriceLine);
    layout->addLayout(AlertTimeLine);
    layout->addWidget(okButton);
    layout->setAlignment(Qt::AlignCenter);

    setLayout(layout);
}

addFuturesUI::~addFuturesUI() = default;