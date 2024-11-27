// CTPQuoteDemo.cpp
#include <iostream>
#include <thread>
#include "CThostFtdcMdSpiImp.h"
#pragma comment(lib, "tradeapi64_se/thostmduserapi_se.lib")

int main()
{
    // 创建行情处理器实例
    CSimpleMdHandler ash;

    // 由于连接是异步的，等待直到登录完成并订阅成功
    // 可以通过更高级的同步机制（如条件变量）来优化等待
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 等待用户输入以保持程序运行
    std::cout << "Press Enter to exit..." << std::endl;
    getchar();

    // 发送登出请求（可选）
    ash.ReqUserLogout();

    return 0;
}