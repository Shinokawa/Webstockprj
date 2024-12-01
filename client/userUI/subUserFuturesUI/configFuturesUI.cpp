#include "configFuturesUI.h"
#include <QHBoxLayout>
#include <QPushButton>

configFuturesUI::configFuturesUI(const Futures& Ftrs): Ftrs(Ftrs) {
    okButton = new QPushButton("确定");
    auto layout = new QHBoxLayout();
    layout->addWidget(okButton);

    setLayout(layout);
}

configFuturesUI::~configFuturesUI() = default;

