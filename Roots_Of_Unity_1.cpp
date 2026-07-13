#include <iostream>
#include <string>
#include <algorithm>
#include <windows.h>  // Windows 专用
using namespace std;

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

int main() {
    // 设置控制台输出为 UTF-8（Windows）
    SetConsoleOutputCP(CP_UTF8);
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    if (n <= 0) {
        cout << "Error: n must be a positive integer." << endl;
        return 0;
    }

    for (int k = 0; k < n; ++k) {
        cout << "z" << (k + 1) << " = ";
        if (k == 0) {
            cout << "1" << endl;
            continue;
        }
        // 化简角度
        int num = 2 * k;
        int den = n;
        int g = gcd(num, den);
        num /= g;
        den /= g;

        // 输出 π 和 ·（Unicode）
        cout << "cos(π·" << num << "/" << den << ") + i·sin(π·" << num << "/" << den << ")" << endl;
    }
    return 0;
}