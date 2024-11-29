#include "Futures.h"
#include <QJsonObject>

Futures::Futures(const QJsonObject& FuturesJson) {
    
    ///交易日
    TradingDay = FuturesJson["TradingDay"].toInt();
    ///保留的无效字段
    reserve1 = FuturesJson["reserve1"].toString().toStdString();
    ///交易所代码
    ExchangeID = FuturesJson["ExchangeID"].toString().toStdString();
    ///保留的无效字段
    reserve2 = FuturesJson["reserve2"].toString().toStdString();
    ///最新价
    LastPrice = FuturesJson["LastPrice"].toDouble();
    ///上次结算价
    PreSettlementPrice = FuturesJson["PreSettlementPrice"].toDouble();
    ///昨收盘
    PreClosePrice = FuturesJson["PreClosePrice"].toDouble();
    ///昨持仓量
    PreOpenInterest = FuturesJson["PreOpenInterest"].toDouble();
    ///今开盘
    OpenPrice = FuturesJson["OpenPrice"].toDouble();
    ///最高价
    HighestPrice = FuturesJson["HighestPrice"].toDouble();
    ///最低价
    LowestPrice = FuturesJson["LowestPrice"].toDouble();
    ///数量
    Volume = FuturesJson["Volume"].toDouble();
    ///成交金额
    Turnover = FuturesJson["Turnover"].toDouble();
    ///持仓量
    OpenInterest = FuturesJson["OpenInterest"].toDouble();
    ///今收盘
    ClosePrice = FuturesJson["ClosePrice"].toDouble();
    ///本次结算价
    SettlementPrice = FuturesJson["SettlementPrice"].toDouble();
    ///涨停板价
    UpperLimitPrice = FuturesJson["UpperLimitPrice"].toDouble();
    ///跌停板价
    LowerLimitPrice = FuturesJson["LowerLimitPrice"].toDouble();
    ///昨虚实度
    PreDelta = FuturesJson["PreDelta"].toDouble();
    ///今虚实度
    CurrDelta = FuturesJson["CurrDelta"].toDouble();
    ///最后修改时间
    UpdateTime = FuturesJson["UpdateTime"].toDouble();
    ///最后修改毫秒
    UpdateMillisec = FuturesJson["UpdateMillisec"].toDouble();
    ///申买价一
    BidPrice1 = FuturesJson["BidPrice1"].toDouble();
    ///申买量一
    BidVolume1 = FuturesJson["BidVolume1"].toDouble();
    ///申卖价一
    AskPrice1 = FuturesJson["AskPrice1"].toDouble();
    ///申卖量一
    AskVolume1 = FuturesJson["AskVolume1"].toDouble();
    ///申买价二
    BidPrice2 = FuturesJson["BidPrice2"].toDouble();
    ///申买量二
    BidVolume2 = FuturesJson["BidVolume2"].toDouble();
    ///申卖价二
    AskPrice2 = FuturesJson["AskPrice2"].toDouble();
    ///申卖量二
    AskVolume2 = FuturesJson["AskVolume2"].toDouble();
    ///申买价三
    BidPrice3 = FuturesJson["BidPrice3"].toDouble();
    ///申买量三
    BidVolume3 = FuturesJson["BidVolume3"].toDouble();
    ///申卖价三
    AskPrice3 = FuturesJson["AskPrice3"].toDouble();
    ///申卖量三
    AskVolume3 = FuturesJson["AskVolume3"].toDouble();
    ///申买价四
    BidPrice4 = FuturesJson["BidPrice4"].toDouble();
    ///申买量四
    BidVolume4 = FuturesJson["BidVolume4"].toDouble();
    ///申卖价四
    AskPrice4 = FuturesJson["AskPrice4"].toDouble();
    ///申卖量四
    AskVolume4 = FuturesJson["AskVolume4"].toDouble();
    ///申买价五
    BidPrice5 = FuturesJson["BidPrice5"].toDouble();
    ///申买量五
    BidVolume5 = FuturesJson["BidVolume5"].toDouble();
    ///申卖价五
    AskPrice5 = FuturesJson["AskPrice5"].toDouble();
    ///申卖量五
    AskVolume5 = FuturesJson["AskVolume5"].toDouble();
    ///当日均价
    AveragePrice = FuturesJson["AveragePrice"].toDouble();
    ///业务日期
    ActionDay = FuturesJson["ActionDay"].toDouble();
    ///合约代码
    InstrumentID = FuturesJson["InstrumentID"].toString().toStdString();
    ///合约在交易所的代码
    ExchangeInstID = FuturesJson["ExchangeInstID"].toString().toStdString();
    ///上带价
    BandingUpperPrice = FuturesJson["BandingUpperPrice"].toDouble();
    ///下带价
    BandingLowerPrice = FuturesJson["BandingLowerPrice"].toDouble();
    
}

Futures::~Futures() = default;
