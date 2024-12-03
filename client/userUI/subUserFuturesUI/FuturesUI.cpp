#include "FuturesUI.h"
#include "../../myServer.h"
#include "configFuturesUI.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>
#include <QGridLayout>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTimer>

#include "../../mainwindow.h"

FuturesUI::FuturesUI(const userManager& user, const Futures &Ftrs): user(user), Ftrs(Ftrs) {
    // 获取并显示初始数据
    //auto data = InitHttpQReq();

    cfgFtrUI = new configFuturesUI(Ftrs);

    exitButton = new QPushButton("退出");
    editButton = new QPushButton("编辑");
    deleteButton = new QPushButton("删除");
    // 名称
    name = new QLabel(Ftrs.InstrumentID.c_str());
    // 代号
    code = new QLabel(Ftrs.ExchangeID.c_str());
    // 最新价
    lastPrice = new QLabel();
    // 涨跌金额
    increaseAmount = new QLabel();
    // 涨跌幅
    increaseRange = new QLabel();
    // 最高价
    highPrice = new QLabel();
    // 最低价
    lowPrice = new QLabel();
    // 开盘价
    OpenPrice = new QLabel();
    // 市值
    marketValue = new QLabel();
    // 昨虚实度
    preDelta = new QLabel();
    // 今虚实度
    currDelta = new QLabel();
    // 涨停板价
    upLimitPrice = new QLabel();
    // 跌停板价
    lowLimitPrice = new QLabel();
    // 成交金额
    transactionNumber = new QLabel();
    // 最近五个买入
    buy1 = new QLabel();
    buy2 = new QLabel();
    buy3 = new QLabel();
    buy4 = new QLabel();
    buy5 = new QLabel();
    // 最近五个卖出
    sell1 = new QLabel();
    sell2 = new QLabel();
    sell3 = new QLabel();
    sell4 = new QLabel();
    sell5 = new QLabel();

    initQLabelData();

    gridLayout = new QGridLayout();

    // 设置按钮样式
    exitButton->setStyleSheet(R"(
        QPushButton {
            background-color: #ccfff5;  /* 按钮背景颜色 */
            color: black;              /* 按钮文字颜色 */
            font-size: 25px;           /* 字体大小 */
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

    editButton->setStyleSheet(R"(
        QPushButton {
            background-color: #ccfff5;  /* 按钮背景颜色 */
            color: black;              /* 按钮文字颜色 */
            font-size: 25px;           /* 字体大小 */
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

    deleteButton->setStyleSheet(R"(
        QPushButton {
            background-color: #ff7070;  /* 按钮背景颜色 */
            color: black;              /* 按钮文字颜色 */
            font-size: 25px;           /* 字体大小 */
            border: none;              /* 无边框 */
            border-radius: 10px;       /* 圆角效果 */
            padding: 5px 15px;         /* 内边距 */
        }
        QPushButton:hover {
            background-color: #ff0000; /* 鼠标悬停颜色 */
        }
        QPushButton:pressed {
            background-color: #cf0000; /* 按下颜色 */
        }
    )");

    // 设置字体
    QFont labelFont("Arial", 15, QFont::Bold);
    name->setFont(labelFont);
    code->setFont(labelFont);
    lastPrice->setFont(labelFont);
    increaseAmount->setFont(labelFont);
    increaseRange->setFont(labelFont);
    highPrice->setFont(labelFont);
    lowPrice->setFont(labelFont);
    OpenPrice->setFont(labelFont);
    marketValue->setFont(labelFont);
    preDelta->setFont(labelFont);
    currDelta->setFont(labelFont);
    upLimitPrice->setFont(labelFont);
    lowLimitPrice->setFont(labelFont);
    transactionNumber->setFont(labelFont);
    buy1->setFont(labelFont);
    buy2->setFont(labelFont);
    buy3->setFont(labelFont);
    buy4->setFont(labelFont);
    buy5->setFont(labelFont);
    sell1->setFont(labelFont);
    sell2->setFont(labelFont);
    sell3->setFont(labelFont);
    sell4->setFont(labelFont);
    sell5->setFont(labelFont);

    // 设置样式
    name->setStyleSheet("color: #1E90FF; background-color: #F0F8FF; border: 1px solid #B0C4DE; padding: 5px;");
    code->setStyleSheet("color: #32CD32; background-color: #F5F5F5; border: 1px solid #B0C4DE; padding: 5px;");
    lastPrice->setStyleSheet("color: #FF6347; background-color: #FAEBD7; border: 1px solid #B0C4DE; padding: 5px;");
    increaseAmount->setStyleSheet("color: #FF4500; background-color: #FFFACD; border: 1px solid #B0C4DE; padding: 5px;");
    increaseRange->setStyleSheet("color: #FFD700; background-color: #FFF8DC; border: 1px solid #B0C4DE; padding: 5px;");
    highPrice->setStyleSheet("color: #008000; background-color: #F0FFF0; border: 1px solid #B0C4DE; padding: 5px;");
    lowPrice->setStyleSheet("color: #800000; background-color: #FFF0F5; border: 1px solid #B0C4DE; padding: 5px;");
    OpenPrice->setStyleSheet("color: #000080; background-color: #FAFAD2; border: 1px solid #B0C4DE; padding: 5px;");
    marketValue->setStyleSheet("color: #DAA520; background-color: #FFF8F0; border: 1px solid #B0C4DE; padding: 5px;");
    preDelta->setStyleSheet("color: #A52A2A; background-color: #F5FFFA; border: 1px solid #B0C4DE; padding: 5px;");
    currDelta->setStyleSheet("color: #8B0000; background-color: #F0E68C; border: 1px solid #B0C4DE; padding: 5px;");
    upLimitPrice->setStyleSheet("color: #2E8B57; background-color: #FFFFE0; border: 1px solid #B0C4DE; padding: 5px;");
    lowLimitPrice->setStyleSheet("color: #B22222; background-color: #FAF0E6; border: 1px solid #B0C4DE; padding: 5px;");
    transactionNumber->setStyleSheet("color: #4682B4; background-color: #F0F8FF; border: 1px solid #B0C4DE; padding: 5px;");
    buy1->setStyleSheet("color: #3CB371; background-color: #F5FFFA; border: 1px solid #B0C4DE; padding: 5px;");
    buy2->setStyleSheet("color: #3CB371; background-color: #F5FFFA; border: 1px solid #B0C4DE; padding: 5px;");
    buy3->setStyleSheet("color: #3CB371; background-color: #F5FFFA; border: 1px solid #B0C4DE; padding: 5px;");
    buy4->setStyleSheet("color: #3CB371; background-color: #F5FFFA; border: 1px solid #B0C4DE; padding: 5px;");
    buy5->setStyleSheet("color: #3CB371; background-color: #F5FFFA; border: 1px solid #B0C4DE; padding: 5px;");
    sell1->setStyleSheet("color: #FF6347; background-color: #FAEBD7; border: 1px solid #B0C4DE; padding: 5px;");
    sell2->setStyleSheet("color: #FF6347; background-color: #FAEBD7; border: 1px solid #B0C4DE; padding: 5px;");
    sell3->setStyleSheet("color: #FF6347; background-color: #FAEBD7; border: 1px solid #B0C4DE; padding: 5px;");
    sell4->setStyleSheet("color: #FF6347; background-color: #FAEBD7; border: 1px solid #B0C4DE; padding: 5px;");
    sell5->setStyleSheet("color: #FF6347; background-color: #FAEBD7; border: 1px solid #B0C4DE; padding: 5px;");

    // 设置文本居中
    QLabel *labels[] = {name, code, lastPrice, increaseAmount, increaseRange, highPrice, lowPrice, OpenPrice, marketValue, preDelta, currDelta, upLimitPrice, lowLimitPrice, transactionNumber, buy1, buy2, buy3, buy4, buy5, sell1, sell2, sell3, sell4, sell5};
    for (QLabel *label : labels) {
        label->setAlignment(Qt::AlignCenter);  // 居中对齐
    }

    // 设置间距
    gridLayout->setHorizontalSpacing(10);  // 水平间距
    gridLayout->setVerticalSpacing(10);    // 垂直间距

    // 添加控件到布局中
    gridLayout->addWidget(exitButton, 0, 0, 2, 1, Qt::AlignCenter);
    gridLayout->addWidget(editButton, 0, 6, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(deleteButton,1,6,1,1,Qt::AlignCenter);
    gridLayout->addWidget(name, 0, 1, 1, 5, Qt::AlignCenter);
    gridLayout->addWidget(code, 1, 1, 1, 5, Qt::AlignCenter);
    gridLayout->addWidget(lastPrice, 2, 0, 2, 2, Qt::AlignCenter);
    gridLayout->addWidget(highPrice, 2, 2, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(marketValue, 2, 3, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(upLimitPrice, 2, 4, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(increaseAmount, 4, 0, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(increaseRange, 4, 1, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(lowPrice, 3, 2, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(preDelta, 3, 3, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(lowLimitPrice, 3, 4, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(OpenPrice, 4, 2, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(currDelta, 4, 3, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(transactionNumber, 4, 4, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(sell1, 2, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(sell2, 3, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(sell3, 4, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(sell4, 5, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(sell5, 6, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(buy1, 7, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(buy2, 8, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(buy3, 9, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(buy4, 10, 5, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(buy5, 11, 5, 1, 2, Qt::AlignCenter);

    gridWidget = new QWidget();
    gridWidget->setLayout(gridLayout);

    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(gridWidget);
    stackedWidget->addWidget(cfgFtrUI);
    stackedWidget->setCurrentWidget(gridWidget);

    auto *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(stackedWidget);
    this->setLayout(mainLayout);

    connect(this->cfgFtrUI->okButton,&QPushButton::clicked,this,doConfigOKButton);
    connect(this->editButton,QPushButton::clicked,this,doEditButton);

    /*
    // 启动定时器，每5秒钟获取一次聊天数据
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FuturesUI::fetchFuturesData);
    timer->start(5000); // 每5000毫秒（即5秒）执行一次 fetchChatData

    // 异步调用，使用lambda表达式来包装成员函数
    // ReSharper disable once CppNoDiscardExpression
    QtConcurrent::run([this] {
        this->fetchFuturesData(); // 在后台线程调用fetchFuturesData
    });
    */
}

FuturesUI::~FuturesUI() = default;

string FuturesUI::InitHttpQReq() const {
    string url = "127.0.0.1:8786/marketdata?uuid=user-1234";
    string PostURL = "http://127.0.0.1:8786/subscribe";
    string Postdata = R"({
        "uuid" : "user-1234",
        "instruments" : ["m2501", "m2505"]
    })";
    auto data = this->user.getServer()->PostFromURL(PostURL, Postdata);
    return data;
}

void FuturesUI::initQLabelData() const {
    //最新价
    lastPrice->setText(QString::number(Ftrs.LastPrice,'f',2));
    //涨跌金额
    double amount = Ftrs.LastPrice - Ftrs.PreClosePrice;
    increaseAmount->setText(QString::number(amount,'f',2));
    //涨跌幅
    amount = (amount / Ftrs.PreClosePrice) * 100.0;
    increaseRange->setText(QString::number(amount,'f',2) + "%");
    //最高价
    auto str = "高\t" + QString::number(Ftrs.HighestPrice,'f',2);
    highPrice->setText(str);
    //最低价
    str = "低\t" + QString::number(Ftrs.LowestPrice,'f',2);
    lowPrice->setText(str);
    //开盘价
    str = "开\t" + QString::number(Ftrs.OpenPrice,'f',2);
    OpenPrice->setText(str);
    //市值
    amount = Ftrs.LastPrice * Ftrs.OpenInterest;
    if (amount / 10000.0 >= 1) {
        amount /= 10000.0;
        if (amount / 10000.0 >= 1) {
            amount /= 10000.0;
            str = "市值\t" + QString::number(amount,'f',2) + "亿";
        }
        else {
            str = "市值\t" + QString::number(amount,'f',2) + "万";
        }
    }
    else {
        str = "市值\t" + QString::number(amount,'f',2);
    }
    marketValue->setText(str);
    //昨虚实度
    str = "昨虚\t" + QString::number(Ftrs.PreDelta,'f',2);
    preDelta->setText(str);
    //今虚实度
    str = "今虚\t" + QString::number(Ftrs.CurrDelta,'f',2);
    currDelta->setText(str);
    //涨停板价
    str = "涨停\t" + QString::number(Ftrs.UpperLimitPrice,'f',2);
    upLimitPrice->setText(str);
    //跌停板价
    str = "跌停\t" + QString::number(Ftrs.LowerLimitPrice,'f',2);
    lowLimitPrice->setText(str);
    //成交金额
    amount = Ftrs.LastPrice * Ftrs.Volume;
    if (amount / 10000.0 >= 1) {
        amount /= 10000.0;
        if (amount / 10000.0 >= 1) {
            amount /= 10000.0;
            str = "额\t" + QString::number(amount,'f',2) + "亿";
        }
        else {
            str = "额\t" + QString::number(amount,'f',2) + "万";
        }
    }
    else {
        str = "额\t" + QString::number(amount,'f',2);
    }
    transactionNumber->setText(str);
    //最近五个买入
    str = "买一\t" + QString::number(Ftrs.BidPrice1,'f',2) + "\t" + QString::number(Ftrs.BidVolume1);
    buy1->setText(str);
    str = "买二\t" + QString::number(Ftrs.BidPrice2,'f',2) + "\t" + QString::number(Ftrs.BidVolume2);
    buy2->setText(str);
    str = "买三\t" + QString::number(Ftrs.BidPrice3,'f',2) + "\t" + QString::number(Ftrs.BidVolume3);
    buy3->setText(str);
    str = "买四\t" + QString::number(Ftrs.BidPrice4,'f',2) + "\t" + QString::number(Ftrs.BidVolume4);
    buy4->setText(str);
    str = "买五\t" + QString::number(Ftrs.BidPrice5,'f',2) + "\t" + QString::number(Ftrs.BidVolume5);
    buy5->setText(str);
    //最近五个卖出
    str = "卖一\t" + QString::number(Ftrs.AskPrice1,'f',2) + "\t" + QString::number(Ftrs.AskVolume1);
    sell1->setText(str);
    str = "卖二\t" + QString::number(Ftrs.AskPrice2,'f',2) + "\t" + QString::number(Ftrs.AskVolume2);
    sell2->setText(str);
    str = "卖三\t" + QString::number(Ftrs.AskPrice3,'f',2) + "\t" + QString::number(Ftrs.AskVolume3);
    sell3->setText(str);
    str = "卖四\t" + QString::number(Ftrs.AskPrice4,'f',2) + "\t" + QString::number(Ftrs.AskVolume4);
    sell4->setText(str);
    str = "卖五\t" + QString::number(Ftrs.AskPrice5,'f',2) + "\t" + QString::number(Ftrs.AskVolume5);
    sell5->setText(str);
}

void FuturesUI::fetchFuturesData() const {
    // 在后台线程中执行网络请求
    string url = "127.0.0.1:8786/marketdata?uuid=user-1234";

    auto data = this->user.getServer()->GetFromURL(url);
    data += QDateTime::currentDateTime().toString().toStdString();

    // 使用信号更新UI，信号会在主线程中执行
    emit updateFuturesUI(data);

}

void FuturesUI::updateFuturesUI(const string &data) const {
    // 在UI线程中更新标签的文本
   // info->setText(data.c_str());
}

void FuturesUI::doEditButton() {
    this->stackedWidget->setCurrentWidget(this->cfgFtrUI);
}

void FuturesUI::doConfigOKButton() {
    double highPriceWarning = this->cfgFtrUI->highPriceWarningText->text().toDouble();
    double lowPriceWarning = this->cfgFtrUI->lowPriceWarningText->text().toDouble();

    if (highPriceWarning != Ftrs.highPriceWarning) {
        auto info = "成功把预警的最高价设定成" + this->cfgFtrUI->highPriceWarningText->text().toStdString();
        MainWindow::popInfoMessage("成功", info);
    }

    if (lowPriceWarning != Ftrs.lowPriceWarning) {
        auto info = "成功把预警的最低价设定成" + this->cfgFtrUI->lowPriceWarningText->text().toStdString();
        MainWindow::popInfoMessage("成功", info);
    }

    this->Ftrs.highPriceWarning = highPriceWarning;
    this->Ftrs.lowPriceWarning = lowPriceWarning;
    this->stackedWidget->setCurrentWidget(this->gridWidget);
}