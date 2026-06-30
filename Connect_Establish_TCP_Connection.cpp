// ============================================================================
// 模块：Windows Socket 编程（连接阶段 + DNS 解析）
// 功能：通过域名（www.bilibili.com）动态获取 IP，并建立 TCP 连接。
// 物理意义：这是爬虫“出门”的第一步——找到目标服务器的门牌号并敲门。
// ============================================================================

#include <iostream>       
#include <winsock2.h>   
#include <ws2tcpip.h>    // 包含 inet_ntoa、inet_addr 等 IP 地址转换函数（用于打印调试）

// 链接指令：告诉链接器去找 ws2_32.lib。如果漏掉，会出现 LNK2019 未定义引用。
#pragma comment(lib, "ws2_32.lib")

// 使用标准命名空间，避免每次写 std::cout
using namespace std;

int main() {
    
    WSADATA wsaData;

    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Winsock startup failed. Error:" << WSAGetLastError() << endl;
        return 1;
    }
    cout << "Winsock initialized." << endl;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {
        cout << "Socket failed." << endl;
        WSACleanup(); // 必须清理已初始化的 Winsock
        return 1;
    }
    cout << "Socket created. Handle: " << sock << endl;

    // =========================================================================
    // DNS 解析（关键升级 —— 从“死地址”变为“活地址”）
    // =========================================================================
    // 目标域名：www.bilibili.com，而不是一个硬编码的 IP。
    // 为什么？因为 IP 会变（CDN 调度、服务器迁移），写死 IP 一定会遇到 10060 超时。
    const char* target = "www.bilibili.com";

    // gethostbyname：向 DNS 服务器发起 UDP 查询，获取域名对应的 IP 地址列表。
    // 权威定义：返回一个 hostent 结构体指针，包含 h_addr_list（IP 地址列表）。
    // 注意：这是阻塞调用。如果 DNS 服务器没响应，它会卡住，直到超时。
    struct hostent* host = gethostbyname(target);

    // 如果 host 为 nullptr，说明 DNS 解析失败（比如网络断连或域名不存在）。
    if (!host) {
        cout << "DNS lookup failed." << endl;
        closesocket(sock); // 先关套接字
        WSACleanup();       // 再关引擎
        return 1;
    }

    // ---------- 准备“收件人地址结构体” ----------
    // sockaddr_in：用于存储 IPv4 地址和端口号的专用结构体。
    sockaddr_in serverAddr;

    // 1. 填写地址族：必须为 AF_INET。
    serverAddr.sin_family = AF_INET;

    // 2. 填写端口：htons(80) 将主机端口号 80 转换为网络字节序（大端序）。
    //    物理原因：Intel/AMD CPU 是小端序（低位在前），而网络协议要求大端序。
    //    如果不转换，对方读到的是 0x5000（20480），必然触发 10061（连接拒绝）。
    serverAddr.sin_port = htons(80);

    // 3. 填写 IP 地址（核心动作）：
    //    host->h_addr_list[0]：指向第一个 IP 地址的二进制数据（char* 类型）。
    //    memcpy：将这块二进制数据（4 字节，对应 IPv4）复制到 sockaddr_in 中。
    //    为什么不直接赋值？因为 h_addr_list[0] 是 char*，而 sin_addr 是 in_addr 结构体，
    //    直接用指针强转风险很高，memcpy 是最安全、最标准的做法。
    memcpy(&serverAddr.sin_addr, host->h_addr_list[0], host->h_length);

    // ---------- 打印解析出的 IP（辅助调试，让你亲眼看到“活地址”） ----------
    // in_addr：存放二进制 IP 的结构体。
    in_addr ip;
    // host->h_addr_list[0] 指向的是 char*。但内存里存的确实是 4 字节的无符号整数。
    // 强制转换为 unsigned long* 取值，再赋给 ip.s_addr，这是老派 C 语言的标准写法。
    ip.s_addr = *(unsigned long*)host->h_addr_list[0];

    // inet_ntoa：将二进制 IP 转换为点分十进制字符串（如 "183.2.172.185"）。
    // 注意：inet_ntoa 内部使用静态缓冲区，在多线程环境下不安全，但当前单线程没问题。
    cout << "Resolved IP: " << inet_ntoa(ip) << endl;

    // =========================================================================
    // 阶段 4：TCP 三次握手（敲门）
    // =========================================================================
    // connect()：向目标服务器发起 TCP 连接请求。
    // 参数 1: sock —— 我们之前创建的套接字句柄。
    // 参数 2: (sockaddr*)&serverAddr —— 强制类型转换。
    //         历史原因：connect 诞生时还没有 sockaddr_in，它只认更通用的 sockaddr。
    //         虽然两者结构不同，但内存开头的 2 个字节（地址族）是相同的，所以强转可行。
    // 参数 3: sizeof(serverAddr) —— 告诉内核这个结构体有多大，以便正确解析后面的 IP 和端口。
    // 返回值：0 表示成功，SOCKET_ERROR（-1）表示失败。
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        // 获取详细的失败原因。
        // 常见的错误码：
        // 10060 (WSAETIMEDOUT)：你发出的 SYN 包石沉大海（目标 IP 不可达或防火墙拦截）。
        // 10061 (WSAECONNREFUSED)：服务器在 80 端口收到了包，但主动拒绝了（端口未监听）。
        cout << "Connection failed. Error: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 如果走到这里，说明三次握手已经完成！
    // 底层状态：你的 TCP 状态机已经从 SYN_SENT 变为 ESTABLISHED。
    cout << "Connection established!" << endl;

    // =========================================================================
    // 阶段 5：清理资源（退房）
    // =========================================================================
    // closesocket：释放套接字句柄，断开 TCP 连接（执行四次挥手）。
    // 注意：即使不调用，进程退出时系统也会回收，但显式释放是良好习惯。
    closesocket(sock);

    // WSACleanup：卸载网络引擎，释放 WSAStartup 申请的资源。
    WSACleanup();

    return 0;
}