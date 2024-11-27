#include "infoUI.h"
#include "../../userManager.h"
#include <QVBoxLayout>
#include <QLabel>

infoUI::infoUI(userManager user) {
    auto username = "用户名：" + user.getUserName();

    auto *title = new QLabel("个人信息");
    usernameLabel = new QLabel(username.c_str());

    auto *layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(usernameLabel);

    this->setLayout(layout);
}

infoUI::~infoUI() = default;