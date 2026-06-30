// ============================================================================
// 模块：Windows Socket 编程入门（极简内核交互模型）
// 功能：初始化网络引擎，创建一个 TCP 端点（套接字），然后释放资源。
// 物理意义：这是 Windows 下任何网络程序（爬虫、服务器、游戏）的“开天辟地”。
// ============================================================================
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); //由"-lws2_32.lib"指定
    if(result != 0)
    {
        cout << "WSAStartup failed. Error:" << result << endl;
        return 1;
    }

    SOCKET sock; // SOCKET是一个无符号整数 (UINT_PTR) 是套接字 句柄 (索引号) 类型 用于标识套接字的句柄
    sock = socket(AF_INET, SOCK_STREAM, 0); 
    // socket() 是一个系统调用 用于创建一个套接字 并返回一个套接字句柄 (索引号) 用于标识该套接字
    // AF_INET 表示 IPv4 协议
    // SOCK_STREAM 表示 TCP 协议
    // 0 表示使用默认的协议 (0 表示使用 TCP 协议 即 IPPROTO_TCP)



    if(sock == INVALID_SOCKET)
    {
        cout << "socket failed. Error:" << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    // WSAGetLastError() 是一个系统调用 用于获取最近一次 Winsock 函数调用的错误码
    // INVALID_SOCKET 表示套接字句柄无效 (创建失败)
    // 常见错误码:
    // 10093 (WSANOTINITIALISED): 系统未初始化 / 未加载 ws2_32.dll 库
    // 10055 (WSAENOBUFS): 内核内存 (非分页池)不足 无法创建新的套接字对象
    // 10024 (WSAEMFILE): 文件描述符不足 无法创建新的套接字对象 (你的进程打开了太多套接字/文件，达到上限)
    // 10047 (WSAEFFNOSUPPORT): 不支持该协议 (如在没装 IPv6 的系统上用了 AF_INET6 协议)

    // 打印 sock 值 用于调试和确认套接字句柄是否有效
    cout << "Socket created successfully. Socket handle: " << sock << endl;
    // 权威定义：closesocket 关闭一个存在的套接字，释放其相关资源
    closesocket(sock);
    cout << "Socket closed successfully." << endl;
    // 权威定义：WSACleanup 注销 Winsock 库，释放 WSAStartup 申请的所有资源
    WSACleanup();
    cout << "Winsock cleanup completed." << endl;
    
    return 0;
}
