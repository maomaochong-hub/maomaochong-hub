#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  //让链接器 链接 ws2_32.lib 库

using namespace std;

int main()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); //激活网络引擎 初始化 Winsock 库(加载 ws2_32.dll 库 以及 引用计数器)

    if(result != 0)  //如果初始化失败 则返回错误码(0代表成功)
    {
        cout << "WSAStartup failed. Error:" << result << endl;
        return 1; //返回错误码 1 表示初始化失败
    }
    cout << "Winsock initialized successfully." << endl;

    WSACleanup(); //清理 Winsock 库 关闭网络引擎 
    cout << "Winsock cleanup completed." << endl;

    return 0;


}

/*
result != 0 的原因: 
1. WSASYSNOTREADY(10091) 系统未初始化 / 未加载 ws2_32.dll 库
解决办法: 
 1. 确保系统已安装 Windows Socket 库
 2. 等待系统初始化 初始化完成后重试
 3. 重置网络适配器(netsh winsock reset)
2. WSAVERNOTSUPPORTED(10092) 版本不支持 Winsock 2.2 版本
解决办法: 
 1. 升级系统到支持 Winsock 2.2 版本的系统
 2. 使用其他 Winsock 库(如 libwinsock2)
 3. 联系系统管理员 确认系统是否支持 Winsock 2.2 版本
3. WSAEPROCLIM(10067) 进程数超过最大限制 
解决办法: 
 1. 请关闭其他正在运行 Winsock 应用程序
 2. 请检查系统配置 是否允许 Winsock 应用程序 运行
4. WSAEFAULT(10014) 代码错误
解决办法: 
 1. 检查代码是否符合 Winsock 规范
 2. 确保所有 Winsock 函数调用都正确
 3. 联系系统管理员 确认系统是否支持 Winsock 2.2 版本
注意: WSAStartup 与 DllMain 冲突
*/
#if 0
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if(result != 0)
    {
        cout << "WSAStartup failed. Error:" << result << endl;
        if(result == WSASYSNOTREADY)
        {
            cout << "System not ready. Please check if Windows Socket library is installed." << endl;
        }
        else if(result == WSAVERNOTSUPPORTED)
        {
            cout << "Version not supported. Please upgrade your system to support Winsock 2.2." << endl;
        }
        else if(result == WSAEPROCLIM)
        {
            cout << "Process limit reached. Please close other Winsock applications and try again." << endl;
        }
        else if(result == WSAEFAULT)
        {
            cout << "Code error. Please check your code for Winsock compliance." << endl;
        }
        else
        {
            cout << "Unknown error. Please check the error code." << endl;
        }
        return 1; 
    }

    cout << "Winsock initialized successfully." << endl;

    WSACleanup();
    cout << "Winsock cleanup completed." << endl;

    return 0;
}
#endif