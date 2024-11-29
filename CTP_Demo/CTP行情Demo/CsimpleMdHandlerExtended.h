// CSimpleMdHandlerExtended.h
#ifndef CSIMPLEMDHANDLEREXTENDED_H
#define CSIMPLEMDHANDLEREXTENDED_H

#include "CThostFtdcMdSpiImp.h" // 引入原始的处理类
#include <vector>
#include <string>
#include <mutex>

// 自定义的行情数据结构（可以根据需要进行调整）
struct MarketData
{
    std::string TradingDay;
    std::string InstrumentID;
    std::string ExchangeID;
    std::string ExchangeInstID;
    std::string UpdateTime;
    std::string ActionDay;
    int Volume;
    int UpdateMillisec;
    int BidVolume1;
    int AskVolume1;
    // ... 根据需要添加更多字段
    double LastPrice;
    double PreSettlementPrice;
    double PreClosePrice;
    double PreOpenInterest;
    double OpenPrice;
    double HighestPrice;
    double LowestPrice;
    double Turnover;
    double OpenInterest;
    double ClosePrice;
    double SettlementPrice;
    double UpperLimitPrice;
    double LowerLimitPrice;
    double PreDelta;
    double CurrDelta;
    double BidPrice1;
    double AskPrice1;
    double BidPrice2;
    double AskPrice2;
    double BidPrice3;
    double AskPrice3;
    double BidPrice4;
    double AskPrice4;
    double BidPrice5;
    double AskPrice5;
    double AveragePrice;
};

// 扩展的行情处理类
class CSimpleMdHandlerExtended : public CSimpleMdHandler
{
public:
    CSimpleMdHandlerExtended() : CSimpleMdHandler() {}
    ~CSimpleMdHandlerExtended() {}

    // 重写行情返回函数以存储数据
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) override
    {
        // 先调用基类的处理（可选）
        CSimpleMdHandler::OnRtnDepthMarketData(pDepthMarketData);

        if (pDepthMarketData)
        {
            MarketData data;
            data.TradingDay = pDepthMarketData->TradingDay;
            data.InstrumentID = pDepthMarketData->InstrumentID;
            data.ExchangeID = pDepthMarketData->ExchangeID;
            data.ExchangeInstID = pDepthMarketData->ExchangeInstID;
            data.UpdateTime = pDepthMarketData->UpdateTime;
            data.ActionDay = pDepthMarketData->ActionDay;
            data.Volume = pDepthMarketData->Volume;
            data.UpdateMillisec = pDepthMarketData->UpdateMillisec;
            data.BidVolume1 = pDepthMarketData->BidVolume1;
            data.AskVolume1 = pDepthMarketData->AskVolume1;
            // ... 填充其他字段
            data.LastPrice = (pDepthMarketData->LastPrice > 10000000) ? 0 : pDepthMarketData->LastPrice;
            data.PreSettlementPrice = (pDepthMarketData->PreSettlementPrice > 10000000) ? 0 : pDepthMarketData->PreSettlementPrice;
            data.PreClosePrice = (pDepthMarketData->PreClosePrice > 10000000) ? 0 : pDepthMarketData->PreClosePrice;
            data.PreOpenInterest = (pDepthMarketData->PreOpenInterest > 10000000) ? 0 : pDepthMarketData->PreOpenInterest;
            data.OpenPrice = (pDepthMarketData->OpenPrice > 10000000) ? 0 : pDepthMarketData->OpenPrice;
            data.HighestPrice = (pDepthMarketData->HighestPrice > 10000000) ? 0 : pDepthMarketData->HighestPrice;
            data.LowestPrice = (pDepthMarketData->LowestPrice > 10000000) ? 0 : pDepthMarketData->LowestPrice;
            data.Turnover = (pDepthMarketData->Turnover > 10000000) ? 0 : pDepthMarketData->Turnover;
            data.OpenInterest = (pDepthMarketData->OpenInterest > 10000000) ? 0 : pDepthMarketData->OpenInterest;
            data.ClosePrice = (pDepthMarketData->ClosePrice > 10000000) ? 0 : pDepthMarketData->ClosePrice;
            data.SettlementPrice = (pDepthMarketData->SettlementPrice > 10000000) ? 0 : pDepthMarketData->SettlementPrice;
            data.UpperLimitPrice = (pDepthMarketData->UpperLimitPrice > 10000000) ? 0 : pDepthMarketData->UpperLimitPrice;
            data.LowerLimitPrice = (pDepthMarketData->LowerLimitPrice > 10000000) ? 0 : pDepthMarketData->LowerLimitPrice;
            data.PreDelta = (pDepthMarketData->PreDelta > 10000000) ? 0 : pDepthMarketData->PreDelta;
            data.CurrDelta = (pDepthMarketData->CurrDelta > 10000000) ? 0 : pDepthMarketData->CurrDelta;
            data.BidPrice1 = (pDepthMarketData->BidPrice1 > 10000000) ? 0 : pDepthMarketData->BidPrice1;
            data.AskPrice1 = (pDepthMarketData->AskPrice1 > 10000000) ? 0 : pDepthMarketData->AskPrice1;
            data.BidPrice2 = (pDepthMarketData->BidPrice2 > 10000000) ? 0 : pDepthMarketData->BidPrice2;
            data.AskPrice2 = (pDepthMarketData->AskPrice2 > 10000000) ? 0 : pDepthMarketData->AskPrice2;
            data.BidPrice3 = (pDepthMarketData->BidPrice3 > 10000000) ? 0 : pDepthMarketData->BidPrice3;
            data.AskPrice3 = (pDepthMarketData->AskPrice3 > 10000000) ? 0 : pDepthMarketData->AskPrice3;
            data.BidPrice4 = (pDepthMarketData->BidPrice4 > 10000000) ? 0 : pDepthMarketData->BidPrice4;
            data.AskPrice4 = (pDepthMarketData->AskPrice4 > 10000000) ? 0 : pDepthMarketData->AskPrice4;
            data.BidPrice5 = (pDepthMarketData->BidPrice5 > 10000000) ? 0 : pDepthMarketData->BidPrice5;
            data.AskPrice5 = (pDepthMarketData->AskPrice5 > 10000000) ? 0 : pDepthMarketData->AskPrice5;
            data.AveragePrice = (pDepthMarketData->AveragePrice > 10000000) ? 0 : pDepthMarketData->AveragePrice;

            // 存储数据（线程安全）
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_marketData.push_back(data);
                // 如果需要限制存储量，可以在这里进行控制，例如：
                // if (m_marketData.size() > MAX_SIZE) { m_marketData.erase(m_marketData.begin()); }
            }
        }

    // 提供获取行情数据的接口
    public:
        // 获取所有存储的行情数据
        std::vector<MarketData> GetMarketData()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_marketData;
        }

        // 清空存储的行情数据
        void ClearMarketData()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_marketData.clear();
        }

    private:
        std::vector<MarketData> m_marketData; // 存储行情数据
        std::mutex m_mutex; // 互斥量保护数据
};

#endif // CSIMPLEMDHANDLEREXTENDED_H