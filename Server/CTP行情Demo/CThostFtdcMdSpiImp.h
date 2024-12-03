#include "tradeapi64_se/ThostFtdcMdApi.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "include/httplib.h" // 纭繚鍖呭惈httplib澶存枃浠�
#include <unordered_set>
#include <set>

// 琛屾儏鏁版嵁瀛樺偍
extern std::mutex data_mutex;
extern std::unordered_map<std::string, CThostFtdcDepthMarketDataField> market_data_map;

using std::cout;
using std::endl;


// 警报设置结构
struct AlertSettings {
    std::string time_alert;          // 格式："HH:MM:SS"
    double resistance_price;         // 压力位价格
    double support_price;            // 支撑位价格
};

// 警报状态结构
struct AlertStatus {
    bool time_alert_triggered = false;
    bool resistance_alert_triggered = false;
    bool support_alert_triggered = false;
    std::vector<std::string> alert_history; // 最近五次警报状态
};

// 全局警报管理变量
extern std::mutex alert_mutex;
extern std::unordered_map<std::string, AlertSettings> client_alert_settings;
extern std::unordered_map<std::string, AlertStatus> client_alert_status;
extern std::unordered_map<std::string, std::unordered_set<std::string>> client_subscriptions;


//行情类
class CSimpleMdHandler : public CThostFtdcMdSpi
{
public:
    // 鏋勯�犲嚱鏁帮紝闇�瑕佷竴涓湁鏁堢殑鎸囧悜CThostFtdcMduserApi瀹炰緥鐨勬寚閽�
    CSimpleMdHandler()
    {
        /// 鍒涘缓api
        m_pUserMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
        if (m_pUserMdApi)
        {
            /// 娉ㄥ唽spi
            m_pUserMdApi->RegisterSpi(this);
            /// 娉ㄥ唽鍓嶇疆
            m_pUserMdApi->RegisterFront(const_cast<char*>("tcp://180.168.146.187:10211"));
            /// 鍒濆鍖�
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

    // 褰撳鎴风涓庝氦鏄撴墭绠＄郴缁熼�氫俊杩炴帴鏂紑鏃讹紝璇ユ柟娉曡璋冪敤
    virtual void OnFrontDisconnected(int nReason)
    {
        // 褰撳彂鐢熻繖涓儏鍐靛悗锛孉PI浼氳嚜鍔ㄩ噸鏂拌繛鎺ワ紝瀹㈡埛绔彲涓嶅仛澶勭悊
        printf("<OnFrontDisconnected>\n");
        printf("\tnReason= = [%d]", nReason);
        printf("</OnFrontDisconnected>\n");
    }

    // 褰撳鎴风鍙戝嚭鐧诲綍璇锋眰涔嬪悗锛岃鏂规硶浼氳璋冪敤锛岄�氱煡瀹㈡埛绔櫥褰曟槸鍚︽垚鍔�
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
            // 绔櫥澶辫触锛屽鎴风闇�杩涜閿欒澶勭悊
            printf("\tFailed to login, errorcode=%d errormsg=%s requestid=%d chain = %d",
                pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
            std::cin.get();
            std::cin.ignore();
            exit(-1);
        }
    }

    ///鐧诲嚭璇锋眰鍝嶅簲
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
            printf((nResult == 0) ? "璁㈤槄琛屾儏璇锋眰......鍙戦�佹垚鍔焅n" : "璁㈤槄琛屾儏璇锋眰......鍙戦�佸け璐ワ紝閿欒搴忓彿=[%d]\n", nResult);
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

    void SubscribeMarketData(const std::vector<std::string>& vecInstrumentID)//鏀惰鎯�
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
            printf((nResult == 0) ? "璁㈤槄琛屾儏璇锋眰......鍙戦�佹垚鍔焅n" : "璁㈤槄琛屾儏璇锋眰......鍙戦�佸け璐ワ紝閿欒搴忓彿=[%d]\n", nResult);
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

    ///娣卞害琛屾儏閫氱煡
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
    {
        if (pDepthMarketData)
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            market_data_map[pDepthMarketData->InstrumentID] = *pDepthMarketData;
        }

        // 获取系统时间

        auto tmNow = std::chrono::system_clock::now();
        std::time_t time_now = std::chrono::system_clock::to_time_t(tmNow);
        std::tm local_tm;
        if (localtime_s(&local_tm, &time_now) != 0) {
            // 处理错误
            std::cerr << "Failed to convert time using localtime_s." << std::endl;
            return;
        }
        char current_time[9]; // HH:MM:SS
        std::strftime(current_time, sizeof(current_time), "%H:%M:%S", &local_tm);
        std::string current_time_str(current_time);

        // 检查所有用户的警报
        std::lock_guard<std::mutex> lock(alert_mutex);
        for (auto& [uuid, settings] : client_alert_settings)
        {
            AlertStatus& status = client_alert_status[uuid];
            bool alert_triggered = false;
            std::string alert_message;

            // 时间警报
            if (!settings.time_alert.empty() && !status.time_alert_triggered && current_time_str == settings.time_alert)
            {
                status.time_alert_triggered = true;
                alert_triggered = true;
                alert_message += "Time alert triggered at " + settings.time_alert + "; ";
            }

            // 检查用户是否订阅了当前Instrument
            if (client_subscriptions.find(uuid) != client_subscriptions.end() &&
                client_subscriptions[uuid].find(pDepthMarketData->InstrumentID) != client_subscriptions[uuid].end())
            {
                double last_price = pDepthMarketData->LastPrice;

                // 压力位警报
                if (settings.resistance_price > 0 && !status.resistance_alert_triggered && last_price >= settings.resistance_price)
                {
                    status.resistance_alert_triggered = true;
                    alert_triggered = true;
                    alert_message += "Resistance price alert triggered at " + std::to_string(settings.resistance_price) + "; ";
                }

                // 支撑位警报
                if (settings.support_price > 0 && !status.support_alert_triggered && last_price <= settings.support_price)
                {
                    status.support_alert_triggered = true;
                    alert_triggered = true;
                    alert_message += "Support price alert triggered at " + std::to_string(settings.support_price) + "; ";
                }
            }

            if (alert_triggered)
            {
                // 记录警报历史，保持五次
                if (status.alert_history.size() >= 5)
                {
                    status.alert_history.erase(status.alert_history.begin());
                }
                status.alert_history.push_back(alert_message);
                printf("Alert for UUID [%s]: %s\n", uuid.c_str(), alert_message.c_str());
            }
        }

        std::tm rtnTimeInfo;
        auto&& timeTemp = std::chrono::system_clock::to_time_t(tmNow);
        localtime_s(&rtnTimeInfo, &timeTemp);
        printf("%02d:%02d:%02d.%03lld\t",
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
        
    };

private:
    // 鎸囧悜CThostFtdcMduserApi瀹炰緥鐨勬寚閽�
    CThostFtdcMdApi* m_pUserMdApi;
};