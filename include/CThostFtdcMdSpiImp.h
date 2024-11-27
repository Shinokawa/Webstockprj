// CThostFtdcMdSpiImp.h
#ifndef CTHOSTFTDCMDSPIIMP_H
#define CTHOSTFTDCMDSPIIMP_H

#include "tradeapi64_se/ThostFtdcMdApi.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>

// 假设有一个PreWarningManager类
// #include "PreWarningManager.h"

using std::cout;
using std::endl;

class CSimpleMdHandler : public CThostFtdcMdSpi
{
public:
    CSimpleMdHandler()
    {
        // 创建并初始化API
        m_pUserMdApi = CThostFtdcMdApi::CreateFtdcMdApi(".//flow_md/", true, true);
        if (m_pUserMdApi)
        {
            // 注册SPI
            m_pUserMdApi->RegisterSpi(this);
            // 注册前置机地址
            m_pUserMdApi->RegisterFront(const_cast<char*>("tcp://180.168.146.187:10211"));
            // 初始化API，开始连接
            m_pUserMdApi->Init();
        }
        else
        {
            std::cerr << "Failed to create CThostFtdcMdApi instance." << std::endl;
            exit(-1);
        }
    }

    ~CSimpleMdHandler()
    {
        if (m_pUserMdApi)
        {
            m_pUserMdApi->Release();
            m_pUserMdApi = nullptr;
        }
    }

    // 前置机连接成功回调
    virtual void OnFrontConnected() override
    {
        printf("<OnFrontConnected>\n");
        // 发送登录请求
        ReqUserLogin();
    }

    // 心跳警告回调
    virtual void OnHeartBeatWarning(int nTimeLapse) override
    {
        printf("<OnHeartBeatWarning>\n");
        printf("\tnTimeLapse [%d]\n", nTimeLapse);
        printf("</OnHeartBeatWarning>\n");
    }

    // 前置机断开连接回调
    virtual void OnFrontDisconnected(int nReason) override
    {
        printf("<OnFrontDisconnected>\n");
        printf("\tnReason= = [%d]\n", nReason);
        printf("</OnFrontDisconnected>\n");
        // TODO: 实现自动重连机制
    }

    // 登录响应回调
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
        CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override
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
        if (pRspInfo && pRspInfo->ErrorID != 0)
        {
            // 登录失败处理
            std::cerr << "Login failed, errorcode=" << pRspInfo->ErrorID
                      << " errormsg=" << pRspInfo->ErrorMsg << std::endl;
            exit(-1);
        }
        else
        {
            // 登录成功后，可以进行订阅
            SubscribeMarketData({"m2501"}); // 示例订阅
        }
    }

    // 登出响应回调
    virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout,
        CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override
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
    }

    // 订阅行情响应回调
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument,
        CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override
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
    }

    // 取消订阅行情响应回调
    virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument,
        CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override
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
    }

    // 接收实时行情数据回调
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) override
    {
        // 获取当前时间
        auto tmNow = std::chrono::system_clock::now();
        std::tm rtnTimeInfo;
        auto&& timeTemp = std::chrono::system_clock::to_time_t(tmNow);
        localtime_r(&timeTemp, &rtnTimeInfo);
        printf("%02d:%02d:%02d.%03lld\t",
            rtnTimeInfo.tm_hour,
            rtnTimeInfo.tm_min,
            rtnTimeInfo.tm_sec,
            std::chrono::duration_cast<std::chrono::milliseconds>(tmNow.time_since_epoch() % std::chrono::seconds(1)).count());
        printf("<OnRtnDepthMarketData>");
        if (pDepthMarketData)
        {
            printf("\tInstrumentID [%s]\tLastPrice [%.8lf]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);
            
            // TODO: 集成预警逻辑，例如：
            // preWarningManager.CheckPriceWarnings(ConvertToInternalFormat(pDepthMarketData));
        }
        printf("</OnRtnDepthMarketData>\n");
    }

    // 注册Fens用户信息（示例中未使用）
    void RegisterFensUserInfo()
    {
        CThostFtdcFensUserInfoField pFens = { 0 };
        // 填充pFens字段
        m_pUserMdApi->RegisterFensUserInfo(&pFens);
    }

    // 发送登录请求
    void ReqUserLogin()
    {
        CThostFtdcReqUserLoginField reqUserLogin = { 0 };
        strcpy(reqUserLogin.BrokerID, "您的BrokerID");
        strcpy(reqUserLogin.UserID, "您的UserID");
        strcpy(reqUserLogin.Password, "您的Password");
        int num = m_pUserMdApi->ReqUserLogin(&reqUserLogin, 2);
        printf("\tlogin num = %d\n", num);
    }

    // 发送登出请求
    void ReqUserLogout()
    {
        CThostFtdcUserLogoutField reqLogout = { 0 };
        strcpy(reqLogout.BrokerID, "您的BrokerID");
        strcpy(reqLogout.UserID, "您的UserID");
        int num = m_pUserMdApi->ReqUserLogout(&reqLogout, 1);
        printf("\tlogout num = %d\n", num);
    }

    // 批量订阅行情数据
    void SubscribeMarketData(const std::vector<std::string>& vecInstrumentID)
    {
        int nIndexReal = 0;
        int nLoopStep = 500;
        for (int nIndexLoop = 0; nIndexLoop <= vecInstrumentID.size() / nLoopStep; nIndexLoop++)
        {
            int nCurrLoopNum = (nIndexLoop < vecInstrumentID.size() / nLoopStep) ? nLoopStep : (vecInstrumentID.size() % nLoopStep);
            auto ppInstrumentID = std::make_unique<char*[]>(nCurrLoopNum);
            for (int nCurrLoopIndex = 0; nCurrLoopIndex < nCurrLoopNum; nCurrLoopIndex++)
            {
                ppInstrumentID[nCurrLoopIndex] = const_cast<char*>(vecInstrumentID.at(nIndexReal).c_str());
                nIndexReal++;
            }
            int nResult = m_pUserMdApi->SubscribeMarketData(ppInstrumentID.get(), nCurrLoopNum);
            printf((nResult == 0) ? "Subscribe Market Data Successful.\n" : "Subscribe Market Data Failed, Error Code=[%d]\n", nResult);
        }
    }

    // 批量取消订阅行情数据
    void UnSubscribeMarketData(const std::vector<std::string>& vecInstrumentID)
    {
        int nIndexReal = 0;
        int nLoopStep = 500;
        for (int nIndexLoop = 0; nIndexLoop <= vecInstrumentID.size() / nLoopStep; nIndexLoop++)
        {
            int nCurrLoopNum = (nIndexLoop < vecInstrumentID.size() / nLoopStep) ? nLoopStep : (vecInstrumentID.size() % nLoopStep);
            auto ppInstrumentID = std::make_unique<char*[]>(nCurrLoopNum);
            for (int nCurrLoopIndex = 0; nCurrLoopIndex < nCurrLoopNum; nCurrLoopIndex++)
            {
                ppInstrumentID[nCurrLoopIndex] = const_cast<char*>(vecInstrumentID.at(nIndexReal).c_str());
                nIndexReal++;
            }
            int nResult = m_pUserMdApi->UnSubscribeMarketData(ppInstrumentID.get(), nCurrLoopNum);
            printf((nResult == 0) ? "UnSubscribe Market Data Successful.\n" : "UnSubscribe Market Data Failed, Error Code=[%d]\n", nResult);
        }
    }

private:
    CThostFtdcMdApi* m_pUserMdApi;
};

#endif // CTHOSTFTDCMDSPIIMP_H