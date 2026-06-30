#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>   // 包含 inet_ntoa 等函数
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    SetConsoleOutputCP(CP_UTF8);
    // ---------- 1. 初始化 Winsock ----------
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup 失败" << std::endl;
        return 1;
    }

    // ---------- 2. 创建 Socket ----------
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cout << "socket 创建失败，错误码: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // ---------- 3. DNS 解析（将域名转为 IP）----------
    const char* hostname = "www.baidu.com";               // 目标域名
    struct hostent* host = gethostbyname(hostname);       // 查询 DNS
    if (host == nullptr) {
        std::cout << "DNS 解析失败，无法找到 " << hostname
                  << "，错误码: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 将解析到的第一个 IP 地址打印出来（方便你观察）
    in_addr ipAddr;
    ipAddr.s_addr = *(unsigned long*)host->h_addr_list[0]; // 取出二进制 IP
    std::cout << "DNS 解析成功，IP: " << inet_ntoa(ipAddr) << std::endl;

    // ---------- 4. 填写服务器地址结构 ----------
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(80);                       // HTTP 端口
    // 将 DNS 返回的 IP 复制到地址结构中
    memcpy(&serverAddr.sin_addr, host->h_addr_list[0], host->h_length);

    // ---------- 5. 连接服务器 ----------
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        int err = WSAGetLastError();                       // 获取具体错误码
        std::cout << "connect 失败，错误码: " << err << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    std::cout << "连接成功！正在发送请求..." << std::endl;

    // ---------- 6. 发送 HTTP 请求 ----------
    std::string request =
        "GET / HTTP/1.1\r\n"
        "Host: www.baidu.com\r\n"
        "Connection: close\r\n"
        "\r\n";

    send(sock, request.c_str(), request.size(), 0);

    // ---------- 7. 接收响应 ----------
    char buffer[4096];
    int bytesReceived;
    std::string fullResponse;

    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        fullResponse += buffer;
    }

    std::cout << "响应长度: " << fullResponse.size() << " 字节" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << fullResponse.substr(0, 500) << std::endl;

    size_t titleStart = fullResponse.find("<title>");
    size_t titleEnd = fullResponse.find("</title>");
    if(titleStart != std::string::npos && titleEnd != std::string::npos)
    {
        std::string title = fullResponse.substr(titleStart + 7, titleEnd - titleStart - 7);
        std::cout << "Title : " << title << std::endl;
    }

    // ---------- 8. 清理资源 ----------
    closesocket(sock);
    WSACleanup();

    return 0;
}