#include "tradeapi64_se/ThostFtdcMdApi.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "httplib.h" // 确保包含httplib头文件
#include <unordered_set>
#include <set>

// 行情数据存储
extern std::mutex data_mutex;
extern std::unordered_map<std::string, CThostFtdcDepthMarketDataField> market_data_map;

using std::cout;
using std::endl;

//行情类
class CSimpleMdHandler : public CThostFtdcMdSpi
{
public:
    // 构造函数，需要一个有效的指向CThostFtdcMduserApi实例的指针
    CSimpleMdHandler()
    {
        /// 创建api
        m_pUserMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
        if (m_pUserMdApi)
        {
            /// 注册spi
            m_pUserMdApi->RegisterSpi(this);
            /// 注册前置
            m_pUserMdApi->RegisterFront(const_cast<char*>("tcp://180.168.146.187:10211"));
            /// 初始化
            m_pUserMdApi->Init();
        }
        else
        {
            exit(-1);
        }
    }
    ~CSimpleMdHandler()
    {
        m_pUserMdApi->Release();
    }


    virtual void OnFrontConnected()
    {
        printf("<OnFrontConnected>\n");
        printf("</OnFrontConnected>\n");
    }

    void RegisterFensUserInfo()
    {
        CThostFtdcFensUserInfoField pFens = { 0 };
        m_pUserMdApi->RegisterFensUserInfo(&pFens);
    }

    void ReqUserLogin()
    {
        CThostFtdcReqUserLoginField reqUserLogin = { 0 };
        int num = m_pUserMdApi->ReqUserLogin(&reqUserLogin, 2);
        printf("\tlogin num = %d\n", num);
    }

    void ReqUserLogout()
    {
        CThostFtdcUserLogoutField a = { 0 };
        int num = m_pUserMdApi->ReqUserLogout(&a, 1);
        printf("\tlogout num = %d\n", num);
    }

    virtual void OnHeartBeatWarning(int nTimeLapse)
    {
        printf("<OnHeartBeatWarning>\n");
        printf("\tnTimeLapse [%d]\n", nTimeLapse);
        printf("</OnHeartBeatWarning>\n");
    }

    // 当客户端与交易托管系统通信连接断开时，该方法被调用
    virtual void OnFrontDisconnected(int nReason)
    {
        // 当发生这个情况后，API会自动重新连接，客户端可不做处理
        printf("<OnFrontDisconnected>\n");
        printf("\tnReason= = [%d]", nReason);
        printf("</OnFrontDisconnected>\n");
    }

    // 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
        CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
    {
        printf("<OnRspUserLogin>\n");
        if (pRspUserLogin)
        {
            printf("\tTradingDay [%s]\n", pRspUserLogin->TradingDay);
            printf("\tLoginTime [%s]\n", pRspUserLogin->LoginTime);
            printf("\tBrokerID [%s]\n", pRspUserLogin->BrokerID);
            printf("\tUserID [%s]\n", pRspUserLogin->UserID);
            printf("\tSystemName [%s]\n", pRspUserLogin->SystemName);
            printf("\tMaxOrderRef [%s]\n", pRspUserLogin->MaxOrderRef);
            printf("\tSHFETime [%s]\n", pRspUserLogin->SHFETime);
            printf("\tDCETime [%s]\n", pRspUserLogin->DCETime);
            printf("\tCZCETime [%s]\n", pRspUserLogin->CZCETime);
            printf("\tFFEXTime [%s]\n", pRspUserLogin->FFEXTime);
            printf("\tINETime [%s]\n", pRspUserLogin->INETime);
            printf("\tFrontID [%d]\n", pRspUserLogin->FrontID);
            printf("\tSessionID [%d]\n", pRspUserLogin->SessionID);
        }
        if (pRspInfo)
        {
            printf("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
            printf("\tErrorID [%d]\n", pRspInfo->ErrorID);
        }
        printf("\tnRequestID [%d]\n", nRequestID);
        printf("\tbIsLast [%d]\n", bIsLast);
        printf("</OnRspUserLogin>\n");
        if (pRspInfo->ErrorID != 0) {
            // 端登失败，客户端需进行错误处理
            printf("\tFailed to login, errorcode=%d errormsg=%s requestid=%d chain = %d",
                pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
            std::cin.get();
            std::cin.ignore();
            exit(-1);
        }
    }

    ///登出请求响应
    virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
    {
        printf("<OnRspUserLogout>\n");
        if (pUserLogout)
        {
            printf("\tBrokerID [%s]\n", pUserLogout->BrokerID);
            printf("\tUserID [%s]\n", pUserLogout->UserID);
        }
        if (pRspInfo)
        {
            printf("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
            printf("\tErrorID [%d]\n", pRspInfo->ErrorID);
        }
        printf("\tnRequestID [%d]\n", nRequestID);
        printf("\tbIsLast [%d]\n", bIsLast);
        printf("</OnRspUserLogout>\n");
        //pUserApi->Release();
    }

    void UnSubscribeMarketData(const std::vector<std::string>& vecInstrumentID)
    {
        int nIndexReal = 0;
        int nLoopStep = 500;
        for (int nIndexLoop = 0; nIndexLoop <= vecInstrumentID.size() / nLoopStep; nIndexLoop++)
        {
            int nCurrLoopNum = nIndexLoop < vecInstrumentID.size() / nLoopStep ? nLoopStep : vecInstrumentID.size() % nLoopStep;
            auto ppInstrumentID = std::make_unique<char* []>(nCurrLoopNum);
            for (int nCurrLoopIndex = 0; nCurrLoopIndex < nCurrLoopNum; nCurrLoopIndex++)
            {
                ppInstrumentID[nCurrLoopIndex] = const_cast<char*>(vecInstrumentID.at(nIndexReal).c_str());
                nIndexReal++;
            }
            int nResult = m_pUserMdApi->UnSubscribeMarketData(ppInstrumentID.get(), nCurrLoopNum);
            printf((nResult == 0) ? "订阅行情请求......发送成功\n" : "订阅行情请求......发送失败，错误序号=[%d]\n", nResult);
        }
    }

    virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
    {
        printf("<OnRspUnSubMarketData>\n");
        if (pSpecificInstrument)
        {
            printf("\tInstrumentID [%s]\n", pSpecificInstrument->InstrumentID);
        }
        if (pRspInfo)
        {
            printf("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
            printf("\tErrorID [%d]\n", pRspInfo->ErrorID);
        }
        printf("\tnRequestID [%d]\n", nRequestID);
        printf("\tbIsLast [%d]\n", bIsLast);
        printf("</OnRspUnSubMarketData>\n");
    };

    void SubscribeMarketData(const std::vector<std::string>& vecInstrumentID)//收行情
    {
        int nIndexReal = 0;
        int nLoopStep = 500;
        for (int nIndexLoop = 0; nIndexLoop <= vecInstrumentID.size() / nLoopStep; nIndexLoop++)
        {
            int nCurrLoopNum = nIndexLoop < vecInstrumentID.size() / nLoopStep ? nLoopStep : vecInstrumentID.size() % nLoopStep;
            auto ppInstrumentID = std::make_unique<char*[]>(nCurrLoopNum);
            for (int nCurrLoopIndex = 0; nCurrLoopIndex < nCurrLoopNum; nCurrLoopIndex++)
            {
                ppInstrumentID[nCurrLoopIndex] = const_cast<char*>(vecInstrumentID.at(nIndexReal).c_str());
                nIndexReal++;
            }
            int nResult = m_pUserMdApi->SubscribeMarketData(ppInstrumentID.get(), nCurrLoopNum);
            printf((nResult == 0) ? "订阅行情请求......发送成功\n" : "订阅行情请求......发送失败，错误序号=[%d]\n", nResult);
        }
    }

    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
    {
        printf("<OnRspSubMarketData>\n");
        if (pSpecificInstrument)
        {
            printf("\tInstrumentID [%s]\n", pSpecificInstrument->InstrumentID);
        }
        if (pRspInfo)
        {
            printf("\tErrorMsg [%s]\n", pRspInfo->ErrorMsg);
            printf("\tErrorID [%d]\n", pRspInfo->ErrorID);
        }
        printf("\tnRequestID [%d]\n", nRequestID);
        printf("\tbIsLast [%d]\n", bIsLast);
        printf("</OnRspSubMarketData>\n");
    };

    ///深度行情通知
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
    {
        if (pDepthMarketData)
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            market_data_map[pDepthMarketData->InstrumentID] = *pDepthMarketData;
        }
        //获取系统时间
        auto tmNow = std::chrono::system_clock::now();
        std::tm rtnTimeInfo;
        auto&& timeTemp = std::chrono::system_clock::to_time_t(tmNow);
        localtime_s(&rtnTimeInfo, &timeTemp);
        /*printf("%02d:%02d:%02d.%03lld\t",
            rtnTimeInfo.tm_hour,
            rtnTimeInfo.tm_min,
            rtnTimeInfo.tm_sec,
            std::chrono::duration_cast<std::chrono::milliseconds>(tmNow.time_since_epoch() % 1000).count());
        printf("<OnRtnDepthMarketData>");
        if (pDepthMarketData)
        {
            printf("\tTradingDay [%s]\n", pDepthMarketData->TradingDay);
            printf("\tInstrumentID [%s]", pDepthMarketData->InstrumentID);
            printf("\tExchangeID [%s]", pDepthMarketData->ExchangeID);
            printf("\tExchangeInstID [%s]\n", pDepthMarketData->ExchangeInstID);
            printf("\tUpdateTime [%s]", pDepthMarketData->UpdateTime);
            printf("\tActionDay [%s]\n", pDepthMarketData->ActionDay);
            printf("\tVolume [%d]\n", pDepthMarketData->Volume);
            printf("\tUpdateMillisec [%d]", pDepthMarketData->UpdateMillisec);
            printf("\tBidVolume1 [%d]\n", pDepthMarketData->BidVolume1);
            printf("\tAskVolume1 [%d]\n", pDepthMarketData->AskVolume1);
            printf("\tBidVolume2 [%d]\n", pDepthMarketData->BidVolume2);
            printf("\tAskVolume2 [%d]\n", pDepthMarketData->AskVolume2);
            printf("\tBidVolume3 [%d]\n", pDepthMarketData->BidVolume3);
            printf("\tAskVolume3 [%d]\n", pDepthMarketData->AskVolume3);
            printf("\tBidVolume4 [%d]\n", pDepthMarketData->BidVolume4);
            printf("\tAskVolume4 [%d]\n", pDepthMarketData->AskVolume4);
            printf("\tBidVolume5 [%d]\n", pDepthMarketData->BidVolume5);
            printf("\tAskVolume5 [%d]\n", pDepthMarketData->AskVolume5);
            printf("\tLastPrice [%.8lf]\n", (pDepthMarketData->LastPrice > 10000000) ? 0 : pDepthMarketData->LastPrice);
            printf("\tPreSettlementPrice [%.8lf]\n", (pDepthMarketData->PreSettlementPrice > 10000000) ? 0 : pDepthMarketData->PreSettlementPrice);
            printf("\tPreClosePrice [%.8lf]\n", (pDepthMarketData->PreClosePrice > 10000000) ? 0 : pDepthMarketData->PreClosePrice);
            printf("\tPreOpenInterest [%.8lf]\n", (pDepthMarketData->PreOpenInterest > 10000000) ? 0 : pDepthMarketData->PreOpenInterest);
            printf("\tOpenPrice [%.8lf]\n", (pDepthMarketData->OpenPrice > 10000000) ? 0 : pDepthMarketData->OpenPrice);
            printf("\tHighestPrice [%.8lf]\n", (pDepthMarketData->HighestPrice > 10000000) ? 0 : pDepthMarketData->HighestPrice);
            printf("\tLowestPrice [%.8lf]\n", (pDepthMarketData->LowestPrice > 10000000) ? 0 : pDepthMarketData->LowestPrice);
            printf("\tTurnover [%.8lf]\n", (pDepthMarketData->Turnover > 10000000) ? 0 : pDepthMarketData->Turnover);
            printf("\tOpenInterest [%.8lf]\n", (pDepthMarketData->OpenInterest > 10000000) ? 0 : pDepthMarketData->OpenInterest);
            printf("\tClosePrice [%.8lf]\n", (pDepthMarketData->ClosePrice > 10000000) ? 0 : pDepthMarketData->ClosePrice);
            printf("\tSettlementPrice [%.8lf]\n", (pDepthMarketData->SettlementPrice > 10000000) ? 0 : pDepthMarketData->SettlementPrice);
            printf("\tUpperLimitPrice [%.8lf]\n", (pDepthMarketData->UpperLimitPrice > 10000000) ? 0 : pDepthMarketData->UpperLimitPrice);
            printf("\tLowerLimitPrice [%.8lf]\n", (pDepthMarketData->LowerLimitPrice > 10000000) ? 0 : pDepthMarketData->LowerLimitPrice);
            printf("\tPreDelta [%.8lf]\n", (pDepthMarketData->PreDelta > 10000000) ? 0 : pDepthMarketData->PreDelta);
            printf("\tCurrDelta [%.8lf]\n", (pDepthMarketData->CurrDelta > 10000000) ? 0 : pDepthMarketData->CurrDelta);
            printf("\tBidPrice1 [%.8lf]\n", (pDepthMarketData->BidPrice1 > 10000000) ? 0 : pDepthMarketData->BidPrice1);
            printf("\tAskPrice1 [%.8lf]\n", (pDepthMarketData->AskPrice1 > 10000000) ? 0 : pDepthMarketData->AskPrice1);
            printf("\tBidPrice2 [%.8lf]\n", (pDepthMarketData->BidPrice2 > 10000000) ? 0 : pDepthMarketData->BidPrice2);
            printf("\tAskPrice2 [%.8lf]\n", (pDepthMarketData->AskPrice2 > 10000000) ? 0 : pDepthMarketData->AskPrice2);
            printf("\tBidPrice3 [%.8lf]\n", (pDepthMarketData->BidPrice3 > 10000000) ? 0 : pDepthMarketData->BidPrice3);
            printf("\tAskPrice3 [%.8lf]\n", (pDepthMarketData->AskPrice3 > 10000000) ? 0 : pDepthMarketData->AskPrice3);
            printf("\tBidPrice4 [%.8lf]\n", (pDepthMarketData->BidPrice4 > 10000000) ? 0 : pDepthMarketData->BidPrice4);
            printf("\tAskPrice4 [%.8lf]\n", (pDepthMarketData->AskPrice4 > 10000000) ? 0 : pDepthMarketData->AskPrice4);
            printf("\tBidPrice5 [%.8lf]\n", (pDepthMarketData->BidPrice5 > 10000000) ? 0 : pDepthMarketData->BidPrice5);
            printf("\tAskPrice5 [%.8lf]\n", (pDepthMarketData->AskPrice5 > 10000000) ? 0 : pDepthMarketData->AskPrice5);
            printf("\tAveragePrice [%.8lf]\n", (pDepthMarketData->AveragePrice > 10000000) ? 0 : pDepthMarketData->AveragePrice);
        }
        printf("</OnRtnDepthMarketData>\n");
        */
    };

private:
    // 指向CThostFtdcMduserApi实例的指针
    CThostFtdcMdApi* m_pUserMdApi;
};