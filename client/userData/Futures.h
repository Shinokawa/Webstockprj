#ifndef FUTURES_H
#define FUTURES_H

#include <QJsonObject>
#include<string>

using namespace std;

class Futures {

public:
	///交易日
	int TradingDay;
	///保留的无效字段
	string reserve1;
	///交易所代码
	string ExchangeID;
	///保留的无效字段
	string reserve2;
	///最新价
	double LastPrice;
	///上次结算价
	double PreSettlementPrice;
	///昨收盘
	double PreClosePrice;
	///昨持仓量
	double PreOpenInterest;
	///今开盘
	double OpenPrice;
	///最高价
	double HighestPrice;
	///最低价
	double LowestPrice;
	///数量
	double Volume;
	///成交金额
	double Turnover;
	///持仓量
	double OpenInterest;
	///今收盘
	double ClosePrice;
	///本次结算价
	double SettlementPrice;
	///涨停板价
	double UpperLimitPrice;
	///跌停板价
	double LowerLimitPrice;
	///昨虚实度
	double PreDelta;
	///今虚实度
	double CurrDelta;
	///最后修改时间
	double UpdateTime;
	///最后修改毫秒
	double UpdateMillisec;
	///申买价一
	double BidPrice1;
	///申买量一
	double BidVolume1;
	///申卖价一
	double AskPrice1;
	///申卖量一
	double AskVolume1;
	///申买价二
	double BidPrice2;
	///申买量二
	double BidVolume2;
	///申卖价二
	double AskPrice2;
	///申卖量二
	double AskVolume2;
	///申买价三
	double BidPrice3;
	///申买量三
	double BidVolume3;
	///申卖价三
	double AskPrice3;
	///申卖量三
	double AskVolume3;
	///申买价四
	double BidPrice4;
	///申买量四
	double BidVolume4;
	///申卖价四
	double AskPrice4;
	///申卖量四
	double AskVolume4;
	///申买价五
	double BidPrice5;
	///申买量五
	double BidVolume5;
	///申卖价五
	double AskPrice5;
	///申卖量五
	double AskVolume5;
	///当日均价
	double AveragePrice;
	///业务日期
	double ActionDay;
	///合约代码
	string InstrumentID;
	///合约在交易所的代码
	string ExchangeInstID;
	///上带价
	double BandingUpperPrice;
	///下带价
	double BandingLowerPrice;

public:
    explicit Futures(const QJsonObject& FuturesJson);
    ~Futures();
};

#endif //FUTURES_H