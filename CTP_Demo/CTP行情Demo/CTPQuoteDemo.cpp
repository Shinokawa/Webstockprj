// main.cpp
#include <iostream>
#include <thread>
#include "CSimpleMdHandlerExtended.h" // 引入扩展后的头文件
#pragma comment(lib, "tradeapi64_se/thostmduserapi_se.lib")

int main()
{
    CSimpleMdHandlerExtended handler;
    /// 由于连接需要时间，且是异步的，此处暂停5s，保证连接完成
    std::this_thread::sleep_for(std::chrono::seconds(5));
    /// 登录
    handler.ReqUserLogin();
    /// 等待，同连接
    std::this_thread::sleep_for(std::chrono::seconds(5));
    /// 订阅行情
    handler.SubscribeMarketData({"m2501"}); // 订阅行情

    // 启动一个线程或在主线程中循环获取数据
    // 这里以简单的示例展示如何获取数据
    std::cout << "开始接收行情数据，按回车键退出...\n";

    while (true)
    {
        // 检查是否有新数据
        {
            auto data = handler.GetMarketData();
            if (!data.empty())
            {
                for (const auto& md : data)
                {
                    std::cout << "InstrumentID: " << md.InstrumentID
                              << ", LastPrice: " << md.LastPrice
                              << ", UpdateTime: " << md.UpdateTime << std::endl;
                    // 根据需要处理数据
                }
                // 获取后可以选择清空存储的数据
                handler.ClearMarketData();
            }
        }

        // 为了避免频繁查询，可以适当睡眠
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // 检查用户是否按下回车键退出
        if (std::cin.peek() != EOF)
        {
            char c = std::cin.get();
            if (c == '\n')
            {
                break;
            }
        }
    }

    return 0;
}