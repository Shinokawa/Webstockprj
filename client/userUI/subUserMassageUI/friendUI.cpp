#include "friendUI.h"
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QJsonArray>
#include <QSplitter>

friendUI::friendUI(const Friend& frd): frd(frd) {
    sendBox = new QTextEdit();
    sendButton = new QPushButton("发送");
    dialogueLayout  = new QVBoxLayout();

    // 获取根对象
    QJsonObject rootObj = this->frd.getMessage();

    // 提取 dialogue 数组
    QJsonArray dialogueArray = rootObj["dialogue"].toArray();

    // 遍历 dialogue 数组
    for (QJsonValue value : dialogueArray) {
        fromJsonLineToLabel(value);
    }

    auto *dialogueWidget = new QWidget();
    dialogueWidget->setLayout(dialogueLayout);  // 对话布局放入 QWidget
    dialogueWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *dialogueArea = new QScrollArea();
    dialogueArea->setWidgetResizable(true);
    dialogueArea->setWidget(dialogueWidget); // 设置滚动区域内容

    auto btnHBox = new QHBoxLayout();
    btnHBox->addStretch();
    btnHBox->addWidget(sendButton);

    auto sendAreaWidget = new QWidget();
    auto sendArea = new QVBoxLayout(sendAreaWidget);
    sendArea->addWidget(sendBox);
    sendArea->addLayout(btnHBox);

    auto splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(dialogueArea);
    splitter->addWidget(sendAreaWidget);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({300, 100});

    auto mainWindow = new QVBoxLayout();
    mainWindow->addWidget(splitter);

    connect(sendButton,&QPushButton::clicked,this,friendUI::doSendButton);

    this->setLayout(mainWindow);
}

friendUI::~friendUI() = default;

void friendUI::fromJsonLineToLabel(const QJsonValue &value) const {
    QJsonObject obj = value.toObject();
    QString sendName = obj["sendName"].toString();
    QString sendMessage = obj["sendMessage"].toString();
    auto messageLayout = new QHBoxLayout();

    if (sendName == "我") {
        auto line = new QLabel(sendMessage);
        line->setStyleSheet("color: blue;");
        line->setAlignment(Qt::AlignRight);
        dialogueLayout->addWidget(line);

        messageLayout->addWidget(line,0,Qt::AlignRight);
    }
    else {
        auto line = new QLabel(sendName + ": " + sendMessage);
        line->setStyleSheet("color: pink;");
        line->setAlignment(Qt::AlignLeft);
        dialogueLayout->addWidget(line);

        messageLayout->addWidget(line,0,Qt::AlignLeft);
    }

    dialogueLayout->addLayout(messageLayout);
}

void friendUI::doSendButton() {
    QString message = sendBox->toPlainText();

    QJsonObject newMessageJson;
    newMessageJson["sendName"] = "我";
    newMessageJson["sendMessage"] = message;

    frd.flashMessage(newMessageJson);

    fromJsonLineToLabel(newMessageJson);

    sendBox->clear();
}
