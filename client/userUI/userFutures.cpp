#include "userFutures.h"
#include "../myServer.h"
#include <QLabel>
#include <QVBoxLayout>

userFutures::userFutures(const userManager& user): user(user) {
    string url = "192.168.111.102:8786/marketdata?uuid=user-1234";
    auto data = this->user.getServer()->GetFromURL(url);
    info = new QLabel(data.c_str());

    auto layout = new QVBoxLayout();
    layout->addWidget(info);
    setLayout(layout);
}

userFutures::~userFutures() = default;
