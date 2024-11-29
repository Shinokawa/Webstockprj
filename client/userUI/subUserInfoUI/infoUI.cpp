#include "infoUI.h"
#include "../../userManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

infoUI::infoUI(userManager user) {
    auto username = "用户名：" + user.getUserName();
    auto EmailAdress = "邮箱地址：" + user.getEmailAdress();

    // 创建标题标签
    auto *title = new QLabel("个人信息");
    title->setFont(QFont("Arial", 18, QFont::Bold));  // 更大、更显眼的标题字体
    title->setAlignment(Qt::AlignCenter);  // 居中对齐标题

    // 创建用户名标签
    usernameLabel = new QLabel(username.c_str());
    usernameLabel->setFont(QFont("Arial", 14));
    usernameLabel->setStyleSheet("background: transparent; color: #333333;");  // 设置文本颜色为深灰

    // 创建邮箱地址标签
    EmailLabel = new QLabel(EmailAdress.c_str());
    EmailLabel->setFont(QFont("Arial", 14));
    EmailLabel->setStyleSheet("background: transparent; color: #333333;");

    // 创建布局并添加标签
    auto *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(10);
    infoLayout->addWidget(usernameLabel);
    infoLayout->addWidget(EmailLabel);

    // 添加透明背景面板
    auto container = new QWidget();
    container->setLayout(infoLayout);
    container->setStyleSheet("background: rgba(255, 255, 255, 0.8); border-radius: 10px; padding: 10px;");

    // 主布局
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);  // 设置控件间距
    mainLayout->addWidget(title);  // 将标题添加到布局
    mainLayout->addSpacing(20);   // 添加标题和信息之间的间距
    mainLayout->addWidget(container, 0, Qt::AlignLeft);  // 将容器添加到布局并居中显示
    mainLayout->addStretch();  // 向下拉伸剩余空间

    // 设置布局
    this->setLayout(mainLayout);
}

infoUI::~infoUI() = default;
