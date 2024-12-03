#ifndef FUTURESUI_H
#define FUTURESUI_H

#include "../../userManager.h"
#include "../../userData/Futures.h"
#include <QWidget>

class QGridLayout;
class QLabel;
class QPushButton;
class configFuturesUI;
class QStackedWidget;

class FuturesUI : public QWidget {

    Q_OBJECT

public:
    userManager user;
    Futures Ftrs;

    QStackedWidget *stackedWidget;

    configFuturesUI *cfgFtrUI;

    QGridLayout *gridLayout;
    QWidget *gridWidget;

    QPushButton *exitButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QLabel *name;   //名称
    QLabel *code;   //代号
    QLabel *lastPrice;  //最新价
    QLabel *increaseAmount; //涨跌金额
    QLabel *increaseRange;  //涨跌幅
    QLabel *highPrice;  //最高价
    QLabel *lowPrice;   //最低价
    QLabel *OpenPrice;  //开盘价
    QLabel *marketValue;    //市值
    QLabel *preDelta; //昨虚实度
    QLabel *currDelta; //今虚实度
    QLabel *upLimitPrice;  //涨停板价
    QLabel *lowLimitPrice;    //跌停板价
    QLabel *transactionNumber;  //成交金额
    //最近五个买入
    QLabel *buy1;
    QLabel *buy2;
    QLabel *buy3;
    QLabel *buy4;
    QLabel *buy5;
    //最近五个卖出
    QLabel *sell1;
    QLabel *sell2;
    QLabel *sell3;
    QLabel *sell4;
    QLabel *sell5;

public:
    explicit FuturesUI(const userManager& user, const Futures &Ftrs);
    ~FuturesUI() override;

    [[nodiscard]] string InitHttpQReq() const;
    void initQLabelData() const;

    void fetchFuturesData() const;  //异步获取期货信息

private slots:
    void updateFuturesUI(const std::string &data) const;   //更新UI的方法

    void doEditButton();
    void doConfigOKButton();
};

#endif //FUTURESUI_H
