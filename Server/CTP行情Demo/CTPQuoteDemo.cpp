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

// 映射UUID到订阅的InstrumentID集合
std::unordered_map<std::string, std::unordered_set<std::string>> client_subscriptions;

// 映射InstrumentID到订阅该Instrument的UUID集合
std::unordered_map<std::string, std::unordered_set<std::string>> instrument_subscribers;

// 互斥锁保护上述映射
std::mutex subscription_mutex;

// 定义一个互斥锁和一个用于存储行情数据的全局map
std::mutex data_mutex;
std::unordered_map<std::string, CThostFtdcDepthMarketDataField> market_data_map;

// 简单的JSON解析函数，用于提取字符串值
std::string extract_json_value(const std::string& json, const std::string& key) {
    size_t key_pos = json.find("\"" + key + "\"");
    if (key_pos == std::string::npos) return "";
    size_t colon = json.find(":", key_pos);
    size_t first_quote = json.find("\"", colon);
    size_t second_quote = json.find("\"", first_quote + 1);
    return json.substr(first_quote + 1, second_quote - first_quote - 1);
}

// 简单的JSON解析函数，用于提取数组值
std::vector<std::string> extract_json_array(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    size_t key_pos = json.find("\"" + key + "\"");
    if (key_pos == std::string::npos) return result;
    size_t colon = json.find(":", key_pos);
    size_t bracket_open = json.find("[", colon);
    size_t bracket_close = json.find("]", bracket_open);
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
            std::string uuid = extract_json_value(body, "uuid");
            std::vector<std::string> instruments = extract_json_array(body, "instruments");

            if (uuid.empty() || instruments.empty()) {
                res.status = 400;
                res.set_content("Invalid request: Missing uuid or instruments", "text/plain");
                return;
            }

            std::vector<std::string> new_subscriptions;
            {
                std::lock_guard<std::mutex> lock(subscription_mutex);
                for (const auto& inst : instruments) {
                    // 如果是新订阅
                    if (client_subscriptions[uuid].insert(inst).second) {
                        instrument_subscribers[inst].insert(uuid);
                        new_subscriptions.push_back(inst);
                    }
                }
            }

            if (!new_subscriptions.empty()) {
                ash.SubscribeMarketData(new_subscriptions);
            }

            res.set_content("Subscribed successfully", "text/plain");
            });

        // 取消订阅端点
        svr.Post("/unsubscribe", [&](const httplib::Request& req, httplib::Response& res) {
            std::string body = req.body;
            std::string uuid = extract_json_value(body, "uuid");
            std::vector<std::string> instruments = extract_json_array(body, "instruments");

            if (uuid.empty() || instruments.empty()) {
                res.status = 400;
                res.set_content("Invalid request: Missing uuid or instruments", "text/plain");
                return;
            }

            std::vector<std::string> to_unsubscribe;
            {
                std::lock_guard<std::mutex> lock(subscription_mutex);
                for (const auto& inst : instruments) {
                    if (client_subscriptions[uuid].erase(inst)) {
                        instrument_subscribers[inst].erase(uuid);
                        // 如果没有任何客户端订阅该Instrument，则取消订阅
                        if (instrument_subscribers[inst].empty()) {
                            to_unsubscribe.push_back(inst);
                        }
                    }
                }
            }

            if (!to_unsubscribe.empty()) {
                ash.UnSubscribeMarketData(to_unsubscribe);
            }

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
            std::lock_guard<std::mutex> lock(subscription_mutex);
            auto client_it = client_subscriptions.find(uuid);
            if (client_it == client_subscriptions.end()) {
                res.status = 404;
                res.set_content("UUID not found", "text/plain");
                return;
            }

            // 手动构建JSON响应
            std::string json = "[";
            bool first = true;
            for (const auto& inst : client_it->second) {
                auto data_it = market_data_map.find(inst);
                if (data_it != market_data_map.end()) {
                    if (!first) {
                        json += ",";
                    }
                    first = false;
                    json += "{";
                    json += "\"InstrumentID\":\"" + std::string(data_it->second.InstrumentID) + "\",";
                    json += "\"LastPrice\":" + std::to_string(data_it->second.LastPrice) + ",";
                    json += "\"Volume\":" + std::to_string(data_it->second.Volume) + ",";
                    json += "\"AveragePrice\":" + std::to_string(data_it->second.AveragePrice);
                    json += "}";
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
