#include "userEmail.h"
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "subUserEmailUI/EmailUI.h"

userEmail::userEmail(const userManager& user) : user(user) {
    stackedWidget = new QStackedWidget();
    listWidget = new QListWidget();
    auto EmailList = this->user.getEmailList();

    // 初始化 listWidget
    for (auto& Eml : EmailList) {
        string option = Eml.getFrom() + "\t\t" + Eml.getSubject();

        listWidget->addItem(option.c_str());

        auto EmlUI = new EmailUI(Eml);

        // 使用 Lambda 绑定槽函数
        connect(EmlUI->exitButton, &QPushButton::clicked, this, [this]() {
            this->doExitButton();
        });

        EmailUIList.emplace_back(EmlUI);
        stackedWidget->addWidget(EmlUI);
    }

    // 美化 listWidget
    listWidget->setStyleSheet(R"(
        QListWidget {
            background-color: #f8f9fa;
            border: none;
        }
        QListWidget::item {
            padding: 10px;
            border-bottom: 1px solid #e1e1e1;
        }
        QListWidget::item:hover {
            background-color: #e9ecef;
        }
        QListWidget::item:selected {
            background-color: #0078D7;
            color: white;
        }
    )");

    // 美化 stackedWidget
    stackedWidget->setStyleSheet("background-color: #ffffff; border-radius: 5px;");

    allStackedWidget = new QStackedWidget();
    allStackedWidget->addWidget(listWidget);
    allStackedWidget->addWidget(stackedWidget);
    allStackedWidget->setCurrentWidget(listWidget);

    // 布局调整
    mainWindow = new QVBoxLayout();
    mainWindow->setContentsMargins(10, 10, 10, 10);  // 设置边距
    mainWindow->addWidget(allStackedWidget);

    connect(listWidget, &QListWidget::itemClicked, this, [this](const QListWidgetItem* item) {
        int row = listWidget->row(item);
        doListWidget(row);
    });

    this->setLayout(mainWindow);
}

userEmail::~userEmail() = default;

void userEmail::doListWidget(int row) const {
    if (row >= 0 && row < EmailUIList.size()) {
        stackedWidget->setCurrentIndex(row);
        allStackedWidget->setCurrentWidget(stackedWidget);
    }
}

void userEmail::doExitButton() const {
    listWidget->clearSelection();
    allStackedWidget->setCurrentWidget(listWidget);
}