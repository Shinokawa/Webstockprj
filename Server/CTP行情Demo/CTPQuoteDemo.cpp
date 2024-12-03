#include <iostream>
#include <thread>
#include "CThostFtdcMdSpiImp.h"
#pragma comment(lib, "tradeapi64_se/thostmduserapi_se.lib")
#include <mutex>
#include <unordered_map>
#include <string>
#include "httplib.h" 
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <windows.h>
#include <sstream>
#include <ws2tcpip.h> 
#pragma comment(lib, "ws2_32.lib") 

std::unordered_map<std::string, std::string> user_emails; // uuid -> email
std::mutex emails_mutex;

// 邮件发送配置
std::string sender_email = "sk_kiko@163.com";    // 在此处设置发送邮箱
std::string sender_password = "KAx7Kzftas4v32JZ";          // 在此处设置发送密码
std::mutex email_config_mutex;

// Base64 编码函数
std::string base64Encode(const std::string& input) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) {
        encoded.push_back(table[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (encoded.size() % 4) {
        encoded.push_back('=');
    }
    return encoded;
}

// 日志宏
#define SMTP_LOG(step, recv_len, buff) \
    if (recv_len >= 0) { \
        std::cout << "[STEP " << step << "] Received: " << std::string(buff, recv_len) << std::endl; \
    } else { \
        std::cerr << "[STEP " << step << "] Error receiving data, ret: " << recv_len << std::endl; \
        return -1; \
    }

int sendEmail(const std::string& user_account,
    const std::string& user_passwd,
    const std::vector<std::string>& dest_account,
    const std::string& content) {
    // SMTP 配置
    std::string smtp_url = "smtp." + user_account.substr(user_account.find('@') + 1);
    int smtp_port = 25; // 明文传输端口

    // 初始化 WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize WinSock." << std::endl;
        return -1;
    }

    // 创建 Socket
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return -1;
    }

    // 使用 getaddrinfo 解析主机名
    struct addrinfo hints = {};
    struct addrinfo* result = nullptr;

    hints.ai_family = AF_INET;         // IPv4
    hints.ai_socktype = SOCK_STREAM;  // 流式套接字
    hints.ai_protocol = IPPROTO_TCP;  // TCP 协议

    if (getaddrinfo(smtp_url.c_str(), std::to_string(smtp_port).c_str(), &hints, &result) != 0) {
        std::cerr << "Failed to resolve host: " << smtp_url << std::endl;
        ::closesocket(sock);
        WSACleanup();
        return -1;
    }

    // 填充 sockaddr_in 结构
    struct sockaddr_in addr = *(struct sockaddr_in*)result->ai_addr;
    freeaddrinfo(result);  // 释放 getaddrinfo 分配的内存

    // 连接服务器
    if (::connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Failed to connect to server." << std::endl;
        ::closesocket(sock);
        WSACleanup();
        return -1;
    }

    char buff[1024] = { 0 };
    size_t len = sizeof(buff);

    // Step 1: Connect
    int recv_len = ::recv(sock, buff, len, 0);
    SMTP_LOG(1, recv_len, buff);

    // Step 2: EHLO
    std::string send_msg = "EHLO localhost\r\n";
    ::send(sock, send_msg.c_str(), send_msg.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    SMTP_LOG(2, recv_len, buff);

    // Step 3: AUTH LOGIN
    send_msg = "AUTH LOGIN\r\n";
    ::send(sock, send_msg.c_str(), send_msg.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    SMTP_LOG(3, recv_len, buff);

    // Step 4: Base64 编码的账号
    std::string account_name = user_account;
    send_msg = base64Encode(account_name) + "\r\n";
    ::send(sock, send_msg.c_str(), send_msg.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    SMTP_LOG(4, recv_len, buff);

    // Step 5: Base64 编码的密码
    send_msg = base64Encode(user_passwd) + "\r\n";
    ::send(sock, send_msg.c_str(), send_msg.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    SMTP_LOG(5, recv_len, buff);

    // Step 6: MAIL FROM
    send_msg = "MAIL FROM: <" + user_account + ">\r\n";
    ::send(sock, send_msg.c_str(), send_msg.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    SMTP_LOG(6, recv_len, buff);

    // Step 7: RCPT TO
    for (const auto& recipient : dest_account) {
        send_msg = "RCPT TO: <" + recipient + ">\r\n";
        ::send(sock, send_msg.c_str(), send_msg.size(), 0);
        recv_len = ::recv(sock, buff, len, 0);
        SMTP_LOG(7, recv_len, buff);
    }

    // Step 8: DATA
    send_msg = "DATA\r\n";
    ::send(sock, send_msg.c_str(), send_msg.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    SMTP_LOG(8, recv_len, buff);

    // Step 9: Content
    send_msg = "FROM: " + user_account + "\r\n";
    send_msg += "TO: ";
    for (const auto& recipient : dest_account) {
        send_msg += recipient + ",";
    }
    send_msg.pop_back(); // Remove trailing comma
    send_msg += "\r\n";
    send_msg += "SUBJECT: 测试邮件\r\n";
    send_msg += "Content-Type: text/plain; charset=utf-8\r\n";
    send_msg += "Content-Transfer-Encoding: base64\r\n\r\n";
    send_msg += base64Encode(content) + "\r\n.\r\n";

    ::send(sock, send_msg.c_str(), send_msg.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    SMTP_LOG(9, recv_len, buff);

    // 关闭 Socket
    ::closesocket(sock);
    WSACleanup();
    return 0;
}



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

// JSON解析函数
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

    // 启动HTTP服务器线程
    std::thread server_thread([&ash]() {
        httplib::Server svr;

        // 订阅端点
        svr.Post("/subscribe", [&](const httplib::Request& req, httplib::Response& res) {
            std::string body = req.body;
            printf("Received subscribe request body: %s\n", body.c_str()); 
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

            {
                std::lock_guard<std::mutex> lock(alert_mutex);
                for (const auto& inst : instruments) {
                    client_subscriptions[uuid].insert(inst);
                }
            }

            {
                std::lock_guard<std::mutex> lock(alert_mutex);
                client_alert_settings[uuid] = AlertSettings{ time_alert, resistance_price, support_price };
                client_alert_status[uuid] = AlertStatus(); 
            }

            ash.SubscribeMarketData(instruments);

            res.set_content("Subscribed successfully", "text/plain");
            });

        // 邮箱端点
        svr.Post("/set_email", [&](const httplib::Request& req, httplib::Response& res) {
            std::string body = req.body;
            printf("Received set_email request body: %s\n", body.c_str()); 

            std::string uuid = extract_json_string(body, "uuid");
            std::string email = extract_json_string(body, "email");

            if (uuid.empty() || email.empty()) {
                res.status = 400;
                res.set_content("Invalid request: Missing uuid or email", "text/plain");
                return;
            }

            {
                std::lock_guard<std::mutex> lock(emails_mutex);
                user_emails[uuid] = email;
            }

            res.set_content("Email set successfully", "text/plain");
            });

        // 取消订阅端点
        svr.Post("/unsubscribe", [&](const httplib::Request& req, httplib::Response& res) {
            std::string body = req.body;
            printf("Received unsubscribe request body: %s\n", body.c_str()); 
            std::string uuid = extract_json_string(body, "uuid");
            std::vector<std::string> instruments = extract_json_array(body, "instruments");

            if (uuid.empty() || instruments.empty()) {
                res.status = 400;
                res.set_content("Invalid request: Missing uuid or instruments", "text/plain");
                return;
            }

            {
                std::lock_guard<std::mutex> lock(alert_mutex);
                for (const auto& inst : instruments) {
                    client_subscriptions[uuid].erase(inst);
                }
                if (client_subscriptions[uuid].empty()) {
                    client_subscriptions.erase(uuid);
                    client_alert_settings.erase(uuid);
                    client_alert_status.erase(uuid);
                }
            }

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
        svr.listen("0.0.0.0", 8786); 
        });

    // 主线程继续运行
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
