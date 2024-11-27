#include "friendUI.h"
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QJsonArray>
#include <QSplitter>

friendUI::friendUI(const Friend& frd) : frd(frd) {
    // 输入框
    sendBox = new QTextEdit();
    sendBox->setPlaceholderText("请输入消息...");
    sendBox->setStyleSheet(
        "border: 1px solid #cccccc;"
        "border-radius: 10px;"
        "padding: 10px;"
        "font-size: 14px;"
        "background-color: #f9f9f9;"
    );

    // 发送按钮
    sendButton = new QPushButton("发送");
    sendButton->setStyleSheet(
        "background-color: #a3e4d7;"
        "color: white;"
        "font-size: 14px;"
        "border: none;"
        "border-radius: 8px;"
        "padding: 8px 20px;"
        "margin-left: 10px;"
        "cursor: pointer;"
    );

    // 对话区域布局
    dialogueLayout = new QVBoxLayout();
    dialogueLayout->setSpacing(10);

    // 从 JSON 加载对话
    QJsonObject rootObj = this->frd.getMessage();
    QJsonArray dialogueArray = rootObj["dialogue"].toArray();
    for (QJsonValue value : dialogueArray) {
        fromJsonLineToLabel(value);
    }

    // 对话区域容器
    auto *dialogueWidget = new QWidget();
    dialogueWidget->setLayout(dialogueLayout);
    dialogueWidget->setStyleSheet("background-color: #ffffff; border-radius: 10px;");
    dialogueWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 滚动区域
    auto *dialogueArea = new QScrollArea();
    dialogueArea->setWidgetResizable(true);
    dialogueArea->setWidget(dialogueWidget);
    dialogueArea->setStyleSheet(
        "background-color: #f2f2f2;"
        "border: none;"
        "border-radius: 10px;"
        "padding: 10px;"
    );

    // 输入区域
    auto btnHBox = new QHBoxLayout();
    btnHBox->addStretch();
    btnHBox->addWidget(sendButton);

    auto sendAreaWidget = new QWidget();
    auto sendArea = new QVBoxLayout(sendAreaWidget);
    sendArea->addWidget(sendBox);
    sendArea->addLayout(btnHBox);

    // 分割器
    auto splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(dialogueArea);
    splitter->addWidget(sendAreaWidget);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({300, 100});

    // 主布局
    auto mainWindow = new QVBoxLayout();
    mainWindow->addWidget(splitter);

    connect(sendButton, &QPushButton::clicked, this, friendUI::doSendButton);

    this->setLayout(mainWindow);
}

friendUI::~friendUI() = default;

void friendUI::fromJsonLineToLabel(const QJsonValue& value) const {
    QJsonObject obj = value.toObject();
    QString sendName = obj["sendName"].toString();
    QString sendMessage = obj["sendMessage"].toString();

    // 气泡框
    auto bubble = new QLabel(sendMessage);
    bubble->setWordWrap(true);
    bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // 单条消息的布局
    auto* messageLayout = new QHBoxLayout();
    if (sendName == "我") {
        bubble->setStyleSheet("background-color: #0078D7; color: white; padding: 10px 15px; border-radius: 10px; font-size: 14px;");
        messageLayout->addStretch();  // 左侧弹性空白
        messageLayout->addWidget(bubble);
    }

    else {
        bubble->setStyleSheet("background-color: #f1f1f1; color: #333333; padding: 10px 15px; border-radius: 10px; font-size: 14px;");
        messageLayout->addWidget(bubble);
        messageLayout->addStretch();  // 右侧弹性空白
    }

    // 单条消息外层容器，用于添加上下间距
    auto messageContainer = new QWidget();
    messageContainer->setLayout(messageLayout);
    messageContainer->setStyleSheet("margin-top: 8px; margin-bottom: 8px;");  // 设置上下间距

    // 将消息添加到对话布局
    dialogueLayout->addWidget(messageContainer);
    dialogueLayout->addStretch();
}

void friendUI::doSendButton() {
    QString message = sendBox->toPlainText();

    if (!message.isEmpty()) {
        QJsonObject newMessageJson;
        newMessageJson["sendName"] = "我";
        newMessageJson["sendMessage"] = message;

        frd.flashMessage(newMessageJson);

        fromJsonLineToLabel(newMessageJson);

        sendBox->clear();
    }
}