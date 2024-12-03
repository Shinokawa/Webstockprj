#include "configFuturesUI.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

configFuturesUI::configFuturesUI(const Futures& Ftrs): Ftrs(Ftrs) {
    auto highPriceWarning = QString::number(this->Ftrs.highPriceWarning,'f',2);
    auto highPriceWarningLabel = new QLabel("最高价预警：");
    highPriceWarningLabel->setFont(QFont("Arial", 12));
    highPriceWarningLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    highPriceWarningText = new QLineEdit(highPriceWarning);
    highPriceWarningText->setStyleSheet("padding: 8px; border-radius: 5px; border: 1px solid #ddd;");

    auto lowPriceWarning = QString::number(this->Ftrs.lowPriceWarning,'f',2);
    auto lowPriceWarningLabel = new QLabel("最低价预警：");
    lowPriceWarningLabel->setFont(QFont("Arial", 12));
    lowPriceWarningLabel->setStyleSheet("background: transparent; border: none; color: #333333; font-size: 14px;");

    lowPriceWarningText = new QLineEdit(lowPriceWarning);
    lowPriceWarningText->setStyleSheet("padding: 8px; border-radius: 5px; border: 1px solid #ddd;");

    this->okButton = new QPushButton("确定");
    okButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px 20px; border-radius: 5px;");

    auto highPriceWarningLine = new QHBoxLayout();
    highPriceWarningLine->addWidget(highPriceWarningLabel);
    highPriceWarningLine->addWidget(highPriceWarningText);

    auto lowPriceWarningLine = new QHBoxLayout();
    lowPriceWarningLine->addWidget(lowPriceWarningLabel);
    lowPriceWarningLine->addWidget(lowPriceWarningText);

    auto layout = new QVBoxLayout();
    layout->setSpacing(20);
    layout->addLayout(highPriceWarningLine);
    layout->addLayout(lowPriceWarningLine);
    layout->addWidget(okButton);
    layout->setAlignment(Qt::AlignCenter);

    auto palette = QPalette(this->palette());
    palette.setColor(QPalette::Window,QColor(245,199,246));
    this->setPalette(palette);
    this->setLayout(layout);
}

configFuturesUI::~configFuturesUI() = default;

