#include "userFutures.h"
#include <QLabel>
#include "../myServer.h"
#include <QHBoxLayout>  // 新增
#include <QStackedWidget>
#include <QListWidget>
#include <QPushButton>

#include "../mainwindow.h"
#include "subUserFuturesUI/FuturesUI.h"

using namespace std;

userFutures::userFutures(const userManager& user): user(user) {
    stackedWidget = new QStackedWidget();
    listWidget = new QListWidget();
    auto FuturesList = this->user.getFuturesList();

    // 设置头标签
    auto headLabel = new QLabel("  名称代码\t\t最新\t\t涨幅\t\t金额");
    headLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    headLabel->setAlignment(Qt::AlignLeft);  // 标签文字对齐方式：左对齐

    // 初始化 listWidget
    for (auto& Ftrs : FuturesList) {

        auto item = initListItem(Ftrs);
        listWidget->addItem(item);

        auto FtrsUI = new FuturesUI(this->user, Ftrs);

        // 使用 Lambda 绑定槽函数
        connect(FtrsUI->exitButton, &QPushButton::clicked, this, [this]() {
            this->doExitButton();
        });

        connect(FtrsUI->deleteButton,&QPushButton::clicked,this, [this, FtrsUI]() {
            this->doDeleteButton(FtrsUI);
        });

        FuturesUIList.emplace_back(FtrsUI);
        stackedWidget->addWidget(FtrsUI);
    }

    // 使用水平布局（QHBoxLayout）对标签与选项对齐
    optionWidget = new QWidget();
    auto optionLayout = new QVBoxLayout(optionWidget);

    // 创建一个水平布局来放置标签与选项
    auto headerLayout = new QHBoxLayout();
    headerLayout->addWidget(headLabel);
    headerLayout->addStretch(1);  // 添加拉伸项，使标签和选项在水平上有合适的间距

    optionLayout->addLayout(headerLayout);  // 使用水平布局
    optionLayout->addWidget(listWidget);

    // 美化 listWidget
    listWidget->setStyleSheet(R"(
        QListWidget {
            background-color: #f8f9fa;
            border: none;
            font-size: 14px;  /* 设置字体大小 */
        }
        QListWidget::item {
            padding: 12px;
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

    // 美化整个界面布局
    allStackedWidget = new QStackedWidget();
    allStackedWidget->addWidget(optionWidget);
    allStackedWidget->addWidget(stackedWidget);
    allStackedWidget->setCurrentWidget(optionWidget);

    // 布局调整
    mainWindow = new QVBoxLayout();
    mainWindow->setContentsMargins(15, 15, 15, 15);  // 设置边距
    mainWindow->addWidget(allStackedWidget);

    connect(listWidget, &QListWidget::itemClicked, this, [this](const QListWidgetItem* item) {
        int row = listWidget->row(item);
        doListWidget(row);
    });

    this->setLayout(mainWindow);
}

userFutures::~userFutures() = default;

QListWidgetItem * userFutures::initListItem(const Futures &Ftrs) {
    double priceChange = ((Ftrs.LastPrice - Ftrs.PreClosePrice) / Ftrs.PreClosePrice) * 100;  //涨幅
    double tradeAmount = Ftrs.LastPrice * Ftrs.Volume;   //金额

    QString qTradeAmount;
    if (tradeAmount / 10000.0 >= 1) {
        tradeAmount /= 10000.0;
        if (tradeAmount / 10000.0 >= 1) {
            tradeAmount /= 10000.0;
            qTradeAmount = QString::number(tradeAmount,'f',2) + "亿";
        }
        else {
            qTradeAmount = QString::number(tradeAmount,'f',2) + "万";
        }
    }
    else {
        qTradeAmount = QString::number(tradeAmount,'f',2);
    }

    auto lastPrice = QString::number(Ftrs.LastPrice,'f',3);
    auto qPriceChange = QString::number(priceChange,'f',2);

    auto option = QString::fromStdString(Ftrs.InstrumentID )+ "\t    " + lastPrice + "\t     " +
                    qPriceChange + "%\t" + qTradeAmount;

    auto *item = new QListWidgetItem(option);
    item->setFont(QFont("Arial", 14)); // 设置字体大小

    // 判断涨跌，设置字体颜色
    if (priceChange > 0) {
        // 涨幅为正，设置字体为绿色
        item->setForeground(QBrush(QColor(255, 0, 0))); // Red
    }
    else if (priceChange < 0) {
        // 涨幅为负，设置字体为红色
        item->setForeground(QBrush(QColor(0, 255, 0))); // Green
    }
    else {
        // 涨幅为零，设置为黑色
        item->setForeground(QBrush(QColor(0, 0, 0))); // Black
    }
    return item;
}

void userFutures::flashFuturesList(const Futures &newFutures) {
    auto item = initListItem(newFutures);
    this->listWidget->addItem(item);
    auto newFuturesUI = new FuturesUI(user, newFutures);

    connect(newFuturesUI->exitButton, &QPushButton::clicked, this, [this]() {
            this->doExitButton();
        });
    connect(newFuturesUI->deleteButton,&QPushButton::clicked,this, [this, newFuturesUI]() {
            this->doDeleteButton(newFuturesUI);
        });

    FuturesUIList.emplace_back(newFuturesUI);
    stackedWidget->addWidget(newFuturesUI);
}

void userFutures::doListWidget(int row) const {
    if (row >= 0 && row < FuturesUIList.size()) {
        stackedWidget->setCurrentIndex(row);
        allStackedWidget->setCurrentWidget(stackedWidget);
    }
}

void userFutures::doExitButton() const {
    listWidget->clearSelection();
    allStackedWidget->setCurrentWidget(optionWidget);
}

void userFutures::doDeleteButton(const FuturesUI* FtrsUI) const {
    auto name = FtrsUI->Ftrs.ExchangeID;

    int index = -1;
    for (int i = 0; i < FuturesUIList.size(); ++i) {
        if (FuturesUIList[i] == FtrsUI) {
            index = i;
            break;
        }
    }

    QListWidgetItem *item = listWidget->takeItem(index);
    delete item; // 删除该项的内存

    FtrsUI = nullptr;  // 防止野指针
    delete FtrsUI;

    listWidget->clearSelection();
    allStackedWidget->setCurrentWidget(optionWidget);
    MainWindow::popInfoMessage("成功","成功删除" + name + "期货");
}