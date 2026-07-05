#include <winsock2.h>
#include <iostream>
#include <string>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")

int main() {
    // 1. 获取目标 IP
    std::string targetIP;
    std::cout << "Enter target computer IP: ";
    std::cin >> targetIP;
    if (targetIP.empty()) {
        std::cerr << "IP cannot be empty!" << std::endl;
        return -1;
    }

    // 2. 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return -1;
    }

    // 3. 创建客户端套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "socket creation failed, error code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // 4. 配置服务端地址
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr(targetIP.c_str());
    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "IP format is invalid!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // 5. 连接服务端
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        int err = WSAGetLastError();
        std::cerr << "Connection failed, error code: " << err << std::endl;
        if (err == WSAETIMEDOUT)
            std::cerr << "→ Timeout: check if IP or server firewall is blocking port 8888" << std::endl;
        else if (err == WSAECONNREFUSED)
            std::cerr << "→ Connection refused: server may not be running or port is not open" << std::endl;
        else if (err == WSAEHOSTUNREACH)
            std::cerr << "→ Host unreachable: check network connectivity" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connection successful! Sending command..." << std::endl;

    // 6. 发送命令
    const char* cmd = "POPUP\n";
    int sendLen = send(clientSocket, cmd, strlen(cmd), 0);
    if (sendLen == SOCKET_ERROR) {
        std::cerr << "Send failed, error code: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "✓ Command sent, waiting for popup from other computer..." << std::endl;
    }

    // 7. 清理
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}