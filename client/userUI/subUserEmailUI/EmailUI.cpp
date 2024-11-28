#include "EmailUI.h"
#include <QLabel>
#include <QScrollArea>
#include <QSplitter>
#include <QHBoxLayout>
#include <QPushButton>

EmailUI::EmailUI(const Email &Eml) : Eml(Eml) {
    // 数据准备
    string From = "发件人：" + this->Eml.getFrom();
    string To = "收件人：" + this->Eml.getTo();
    string Subject = "主题：" + this->Eml.getSubject();
    string Body = this->Eml.getBody();

    // 标题部分（发件人、收件人、主题）
    auto subjectLabel = new QLabel(Subject.c_str());
    auto fromLabel = new QLabel(From.c_str());
    auto toLabel = new QLabel(To.c_str());

    // 设置样式
    subjectLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333;");
    fromLabel->setStyleSheet("background-color: #f5f5f5; padding: 10px; border-radius: 5px;");
    toLabel->setStyleSheet("background-color: #f5f5f5; padding: 10px; border-radius: 5px;");

    // 正文部分
    auto bodyLabel = new QLabel(Body.c_str());
    bodyLabel->setWordWrap(true);  // 自动换行
    bodyLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    bodyLabel->setStyleSheet("padding: 15px; line-height: 1.5; font-size: 16px; color: #333;");

    auto bodyWidget = new QWidget();
    auto bodyLayout = new QVBoxLayout();
    bodyLayout->addWidget(bodyLabel);
    bodyWidget->setLayout(bodyLayout);

    auto *bodyArea = new QScrollArea();
    bodyArea->setWidgetResizable(true);
    bodyArea->setWidget(bodyWidget);
    bodyArea->setStyleSheet("border: none;");  // 去掉滚动区域的边框

    // 退出按钮
    this->exitButton = new QPushButton("退出");
    exitButton->setStyleSheet(R"(
        QPushButton {
            background-color: #EEF7FF;  /* 按钮背景颜色 */
            color: black;              /* 按钮文字颜色 */
            font-size: 16px;           /* 字体大小 */
            border: none;              /* 无边框 */
            border-radius: 10px;       /* 圆角效果 */
            padding: 5px 15px;         /* 内边距 */
        }
        QPushButton:hover {
            background-color: #C3E2FC; /* 鼠标悬停颜色 */
        }
        QPushButton:pressed {
            background-color: #A1D4FF; /* 按下颜色 */
        }
    )");

    // 顶部按钮行
    auto buttonLine = new QHBoxLayout();
    buttonLine->addWidget(exitButton);
    buttonLine->addStretch();

    // 标题布局
    auto titleLayout = new QVBoxLayout();
    titleLayout->addLayout(buttonLine);
    titleLayout->addWidget(subjectLabel);
    titleLayout->addWidget(fromLabel);
    titleLayout->addWidget(toLabel);

    auto titleWidget = new QWidget();
    titleWidget->setLayout(titleLayout);

    // 主布局
    auto splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(titleWidget);
    splitter->addWidget(bodyArea);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 6);

    auto mainWindow = new QVBoxLayout();
    mainWindow->addWidget(splitter);

    this->setLayout(mainWindow);
}

EmailUI::~EmailUI() = default;