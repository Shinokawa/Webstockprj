// CTP行情Demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include "CThostFtdcMdSpiImp.h"
#pragma comment(lib, "tradeapi64_se/thostmduserapi_se.lib")
// 添加到您的主.cpp 或一个公共的头文件中
#include <mutex>
#include <unordered_map>
#include <string>
#include "include/httplib.h" // 包含cpp-httplib头文件

#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <string>



// 映射InstrumentID到订阅该Instrument的UUID集合
std::unordered_map<std::string, std::unordered_set<std::string>> instrument_subscribers;

// 互斥锁保护上述映射
std::mutex subscription_mutex;

// 定义一个互斥锁和一个用于存储行情数据的全局map
std::mutex data_mutex;
std::unordered_map<std::string, CThostFtdcDepthMarketDataField> market_data_map;

// 定义全局警报管理变量
std::mutex alert_mutex;
std::unordered_map<std::string, AlertSettings> client_alert_settings;
std::unordered_map<std::string, AlertStatus> client_alert_status;
std::unordered_map<std::string, std::unordered_set<std::string>> client_subscriptions;

// 简单的JSON解析函数，用于提取字符串值
std::string extract_json_string(const std::string& json, const std::string& key) {
    size_t key_pos = json.find("\"" + key + "\"");
    if (key_pos == std::string::npos) return "";
    size_t colon = json.find(":", key_pos);
    if (colon == std::string::npos) return "";
    size_t first_quote = json.find("\"", colon);
    size_t second_quote = json.find("\"", first_quote + 1);
    if (first_quote == std::string::npos || second_quote == std::string::npos) return "";
    return json.substr(first_quote + 1, second_quote - first_quote - 1);
}

// 简单的JSON解析函数，用于提取数组值
std::vector<std::string> extract_json_array(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    size_t key_pos = json.find("\"" + key + "\"");
    if (key_pos == std::string::npos) return result;
    size_t colon = json.find(":", key_pos);
    if (colon == std::string::npos) return result;
    size_t bracket_open = json.find("[", colon);
    size_t bracket_close = json.find("]", bracket_open);
    if (bracket_open == std::string::npos || bracket_close == std::string::npos) return result;
    std::string array_content = json.substr(bracket_open + 1, bracket_close - bracket_open - 1);

    size_t pos = 0;
    while ((pos = array_content.find("\"")) != std::string::npos) {
        size_t end = array_content.find("\"", pos + 1);
        if (end == std::string::npos) break;
        result.push_back(array_content.substr(pos + 1, end - pos - 1));
        array_content = array_content.substr(end + 1);
    }
    return result;
}

// 简单的JSON解析函数，用于提取双精度数值
double extract_json_double(const std::string& json, const std::string& key) {
    size_t key_pos = json.find("\"" + key + "\"");
    if (key_pos == std::string::npos) return 0.0;
    size_t colon = json.find(":", key_pos);
    if (colon == std::string::npos) return 0.0;
    size_t comma = json.find(",", colon);
    size_t end = (comma != std::string::npos) ? comma : json.find("}", colon);
    if (end == std::string::npos) return 0.0;
    std::string number_str = json.substr(colon + 1, end - colon - 1);
    try {
        return std::stod(number_str);
    }
    catch (...) {
        return 0.0;
    }
}



int main()
{
    // 创建并初始化行情处理器
    CSimpleMdHandler ash;
    ash.RegisterFensUserInfo();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ash.ReqUserLogin();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 移除初始订阅，如果不需要默认订阅
    // ash.SubscribeMarketData({ "m2501","m2505" }); // 订阅行情

    // 启动HTTP服务器线程
    std::thread server_thread([&ash]() {
        httplib::Server svr;

        // 订阅端点
        svr.Post("/subscribe", [&](const httplib::Request& req, httplib::Response& res) {
            std::string body = req.body;
            printf("Received subscribe request body: %s\n", body.c_str()); // 调试日志

            // 解析JSON请求体
            std::string uuid = extract_json_string(body, "uuid");
            std::vector<std::string> instruments = extract_json_array(body, "instruments");
            std::string time_alert = extract_json_string(body, "time_alert");
            double resistance_price = extract_json_double(body, "resistance_price");
            double support_price = extract_json_double(body, "support_price");

            if (uuid.empty() || instruments.empty()) {
                res.status = 400;
                res.set_content("Invalid request: Missing uuid or instruments", "text/plain");
                return;
            }

            // 添加Instrument到用户订阅
            {
                std::lock_guard<std::mutex> lock(alert_mutex);
                for (const auto& inst : instruments) {
                    client_subscriptions[uuid].insert(inst);
                }
            }

            // 存储警报设置
            {
                std::lock_guard<std::mutex> lock(alert_mutex);
                client_alert_settings[uuid] = AlertSettings{ time_alert, resistance_price, support_price };
                client_alert_status[uuid] = AlertStatus(); // 初始化警报状态
            }

            // 订阅行情数据
            ash.SubscribeMarketData(instruments);

            res.set_content("Subscribed successfully", "text/plain");
            });


        // 取消订阅端点
        svr.Post("/unsubscribe", [&](const httplib::Request& req, httplib::Response& res) {
            std::string body = req.body;
            printf("Received unsubscribe request body: %s\n", body.c_str()); // 调试日志

            // 解析JSON请求体
            std::string uuid = extract_json_string(body, "uuid");
            std::vector<std::string> instruments = extract_json_array(body, "instruments");

            if (uuid.empty() || instruments.empty()) {
                res.status = 400;
                res.set_content("Invalid request: Missing uuid or instruments", "text/plain");
                return;
            }

            // 从用户订阅中移除Instrument
            {
                std::lock_guard<std::mutex> lock(alert_mutex);
                for (const auto& inst : instruments) {
                    client_subscriptions[uuid].erase(inst);
                }
                // 如果用户没有任何订阅，移除警报设置和状态
                if (client_subscriptions[uuid].empty()) {
                    client_subscriptions.erase(uuid);
                    client_alert_settings.erase(uuid);
                    client_alert_status.erase(uuid);
                }
            }

            // 取消订阅行情数据
            ash.UnSubscribeMarketData(instruments);

            res.set_content("Unsubscribed successfully", "text/plain");
            });



        // 获取行情数据端点
        svr.Get("/marketdata", [&](const httplib::Request& req, httplib::Response& res) {
            auto it = req.params.find("uuid");
            if (it == req.params.end()) {
                res.status = 400;
                res.set_content("Missing 'uuid' parameter", "text/plain");
                return;
            }

            std::string uuid = it->second;

            // 获取用户的订阅Instrument列表
            std::vector<std::string> subscribed_instruments;
            {
                std::lock_guard<std::mutex> lock(alert_mutex);
                if (client_subscriptions.find(uuid) != client_subscriptions.end()) {
                    for (const auto& inst : client_subscriptions[uuid]) {
                        subscribed_instruments.push_back(inst);
                    }
                }
                else {
                    res.status = 404;
                    res.set_content("UUID not found", "text/plain");
                    return;
                }
            }

            // 构建JSON响应
            std::string json = "[";
            bool first = true;
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                std::lock_guard<std::mutex> lock_alert(alert_mutex);
                for (const auto& inst : subscribed_instruments) {
                    auto data_it = market_data_map.find(inst);
                    if (data_it != market_data_map.end()) {
                        if (!first) {
                            json += ",";
                        }
                        first = false;
                        json += "{";
                        // InstrumentID
                        json += "\"InstrumentID\":\"" + std::string(data_it->second.InstrumentID) + "\",";
                        // TradingDay
                        json += "\"TradingDay\":\"" + std::string(data_it->second.TradingDay) + "\",";
                        // ExchangeID
                        json += "\"ExchangeID\":\"" + std::string(data_it->second.ExchangeID) + "\",";
                        // ExchangeInstID
                        json += "\"ExchangeInstID\":\"" + std::string(data_it->second.ExchangeInstID) + "\",";
                        // UpdateTime
                        json += "\"UpdateTime\":\"" + std::string(data_it->second.UpdateTime) + "\",";
                        // ActionDay
                        json += "\"ActionDay\":\"" + std::string(data_it->second.ActionDay) + "\",";
                        // Volume
                        json += "\"Volume\":" + std::to_string(data_it->second.Volume) + ",";
                        // UpdateMillisec
                        json += "\"UpdateMillisec\":" + std::to_string(data_it->second.UpdateMillisec) + ",";
                        // BidVolume1
                        json += "\"BidVolume1\":" + std::to_string(data_it->second.BidVolume1) + ",";
                        // AskVolume1
                        json += "\"AskVolume1\":" + std::to_string(data_it->second.AskVolume1) + ",";
                        // BidVolume2
                        json += "\"BidVolume2\":" + std::to_string(data_it->second.BidVolume2) + ",";
                        // AskVolume2
                        json += "\"AskVolume2\":" + std::to_string(data_it->second.AskVolume2) + ",";
                        // BidVolume3
                        json += "\"BidVolume3\":" + std::to_string(data_it->second.BidVolume3) + ",";
                        // AskVolume3
                        json += "\"AskVolume3\":" + std::to_string(data_it->second.AskVolume3) + ",";
                        // BidVolume4
                        json += "\"BidVolume4\":" + std::to_string(data_it->second.BidVolume4) + ",";
                        // AskVolume4
                        json += "\"AskVolume4\":" + std::to_string(data_it->second.AskVolume4) + ",";
                        // BidVolume5
                        json += "\"BidVolume5\":" + std::to_string(data_it->second.BidVolume5) + ",";
                        // AskVolume5
                        json += "\"AskVolume5\":" + std::to_string(data_it->second.AskVolume5) + ",";
                        // LastPrice
                        json += "\"LastPrice\":" + std::to_string((data_it->second.LastPrice > 10000000) ? 0 : data_it->second.LastPrice) + ",";
                        // PreSettlementPrice
                        json += "\"PreSettlementPrice\":" + std::to_string((data_it->second.PreSettlementPrice > 10000000) ? 0 : data_it->second.PreSettlementPrice) + ",";
                        // PreClosePrice
                        json += "\"PreClosePrice\":" + std::to_string((data_it->second.PreClosePrice > 10000000) ? 0 : data_it->second.PreClosePrice) + ",";
                        // PreOpenInterest
                        json += "\"PreOpenInterest\":" + std::to_string((data_it->second.PreOpenInterest > 10000000) ? 0 : data_it->second.PreOpenInterest) + ",";
                        // OpenPrice
                        json += "\"OpenPrice\":" + std::to_string((data_it->second.OpenPrice > 10000000) ? 0 : data_it->second.OpenPrice) + ",";
                        // HighestPrice
                        json += "\"HighestPrice\":" + std::to_string((data_it->second.HighestPrice > 10000000) ? 0 : data_it->second.HighestPrice) + ",";
                        // LowestPrice
                        json += "\"LowestPrice\":" + std::to_string((data_it->second.LowestPrice > 10000000) ? 0 : data_it->second.LowestPrice) + ",";
                        // Turnover
                        json += "\"Turnover\":" + std::to_string((data_it->second.Turnover > 10000000) ? 0 : data_it->second.Turnover) + ",";
                        // OpenInterest
                        json += "\"OpenInterest\":" + std::to_string((data_it->second.OpenInterest > 10000000) ? 0 : data_it->second.OpenInterest) + ",";
                        // ClosePrice
                        json += "\"ClosePrice\":" + std::to_string((data_it->second.ClosePrice > 10000000) ? 0 : data_it->second.ClosePrice) + ",";
                        // SettlementPrice
                        json += "\"SettlementPrice\":" + std::to_string((data_it->second.SettlementPrice > 10000000) ? 0 : data_it->second.SettlementPrice) + ",";
                        // UpperLimitPrice
                        json += "\"UpperLimitPrice\":" + std::to_string((data_it->second.UpperLimitPrice > 10000000) ? 0 : data_it->second.UpperLimitPrice) + ",";
                        // LowerLimitPrice
                        json += "\"LowerLimitPrice\":" + std::to_string((data_it->second.LowerLimitPrice > 10000000) ? 0 : data_it->second.LowerLimitPrice) + ",";
                        // PreDelta
                        json += "\"PreDelta\":" + std::to_string((data_it->second.PreDelta > 10000000) ? 0 : data_it->second.PreDelta) + ",";
                        // CurrDelta
                        json += "\"CurrDelta\":" + std::to_string((data_it->second.CurrDelta > 10000000) ? 0 : data_it->second.CurrDelta) + ",";
                        // BidPrice1
                        json += "\"BidPrice1\":" + std::to_string((data_it->second.BidPrice1 > 10000000) ? 0 : data_it->second.BidPrice1) + ",";
                        // AskPrice1
                        json += "\"AskPrice1\":" + std::to_string((data_it->second.AskPrice1 > 10000000) ? 0 : data_it->second.AskPrice1) + ",";
                        // BidPrice2
                        json += "\"BidPrice2\":" + std::to_string((data_it->second.BidPrice2 > 10000000) ? 0 : data_it->second.BidPrice2) + ",";
                        // AskPrice2
                        json += "\"AskPrice2\":" + std::to_string((data_it->second.AskPrice2 > 10000000) ? 0 : data_it->second.AskPrice2) + ",";
                        // BidPrice3
                        json += "\"BidPrice3\":" + std::to_string((data_it->second.BidPrice3 > 10000000) ? 0 : data_it->second.BidPrice3) + ",";
                        // AskPrice3
                        json += "\"AskPrice3\":" + std::to_string((data_it->second.AskPrice3 > 10000000) ? 0 : data_it->second.AskPrice3) + ",";
                        // BidPrice4
                        json += "\"BidPrice4\":" + std::to_string((data_it->second.BidPrice4 > 10000000) ? 0 : data_it->second.BidPrice4) + ",";
                        // AskPrice4
                        json += "\"AskPrice4\":" + std::to_string((data_it->second.AskPrice4 > 10000000) ? 0 : data_it->second.AskPrice4) + ",";
                        // BidPrice5
                        json += "\"BidPrice5\":" + std::to_string((data_it->second.BidPrice5 > 10000000) ? 0 : data_it->second.BidPrice5) + ",";
                        // AskPrice5
                        json += "\"AskPrice5\":" + std::to_string((data_it->second.AskPrice5 > 10000000) ? 0 : data_it->second.AskPrice5) + ",";
                        // AveragePrice
                        json += "\"AveragePrice\":" + std::to_string((data_it->second.AveragePrice > 10000000) ? 0 : data_it->second.AveragePrice) + ",";


                        // 添加警报信息
                        bool has_alert = false;
                        std::vector<std::string> alert_history;
                        auto alert_it = client_alert_status.find(uuid);
                        if (alert_it != client_alert_status.end()) {
                            has_alert = alert_it->second.time_alert_triggered ||
                                alert_it->second.resistance_alert_triggered ||
                                alert_it->second.support_alert_triggered;
                            alert_history = alert_it->second.alert_history;
                        }

                        json += "\"has_alert\":" + std::string(has_alert ? "true" : "false") + ",";
                        json += "\"alert_history\":[";
                        bool first_alert = true;
                        for (const auto& alert : alert_history) {
                            if (!first_alert) {
                                json += ",";
                            }
                            first_alert = false;
                            // 转义双引号
                            std::string escaped_alert = alert;
                            size_t pos = 0;
                            while ((pos = escaped_alert.find("\"", pos)) != std::string::npos) {
                                escaped_alert.insert(pos, "\\");
                                pos += 2;
                            }
                            json += "\"" + escaped_alert + "\"";
                        }
                        json += "]";

                        json += "}";
                    }
                }
            }
            json += "]";

            res.set_content(json, "application/json");
            });


        std::cout << "HTTP服务器启动，监听端口 8786 ..." << std::endl;
        svr.listen("0.0.0.0", 8786); // 监听所有IP的8786端口
        });

    // 主线程继续运行，等待用户输入以退出
    std::cout << "按回车键退出程序..." << std::endl;
    getchar();

    // 释放资源
    ash.ReqUserLogout();

    // 等待服务器线程结束
    if (server_thread.joinable()) {
        server_thread.join();
    }

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
