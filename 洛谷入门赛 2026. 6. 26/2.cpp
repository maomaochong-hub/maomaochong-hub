#if 0
题目描述
题目给定一个到校时刻h，m，表示h时m分（24小时制)。
如果这个时刻早于07:30，你需要输出EARLY。如果这个时刻不早于07:30，并且不晚于08:00，你需要输出ONTIME。如果这个时刻晚于08:00，你需要输出LATE

对于100%的数据，保证：
0 <= h <= 23
0 <= m <= 59
#endif
#if 0
#include<bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0); // 输入输出流
    int h, m;
    cin >> h >> m;
    //映射为分钟
    int Time = h * 60 + m;
    if (Time < 450)
        cout << "EARLY" << endl;
    else if (Time <= 480)
        cout << "ONTIME" << endl;
    else
        cout << "LATE" << endl;

    return 0;
}
#endif
#if 0
把字刻到石头上: "魔数(Magic Number)"  
#include<bits/stdc++.h>
using namespace std;
const int EARLY_LIMIT = 7 * 60 + 30;
const int ONTIME_LIMIT = 8 * 60; //常量定义 单点维护

//逻辑封装 转换为分钟数
int ToMinute(int h, int m)
{
    return h * 60 + m;
}

//逻辑封装 判断状态
string GetStatus(int h, int m)
{
    int Time = ToMinute(h, m);
    if (Time < EARLY_LIMIT)
        return "EARLY";
    else if (Time <= ONTIME_LIMIT)
        return "ONTIME";
    else
        return "LATE";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0); 
    int h, m;
    cin >> h >> m;
    cout << GetStatus(h, m) << endl;

    return 0;
#endif
#if 0
//合法性判断:
#include <bits/stdc++.h>
using namespace std;

const int EARLY_LIMIT = 7 * 60 + 30;
const int ONTIME_LIMIT = 8 * 60;
const int MIN_HOUR = 0;
const int MAX_HOUR = 23;
const int MIN_MINUTE = 0;
const int MAX_MINUTE = 59;
//意图编程: 使思考过程可读性更高
bool isValidTime(int h, int m) 
{
    bool hourValid = (h >= MIN_HOUR) && (h <= MAX_HOUR);
    bool minuteValid = (m >= MIN_MINUTE) && (m <= MAX_MINUTE);
    return hourValid && minuteValid;
}

string getStatus(int minutes) {
    if (minutes < EARLY_LIMIT) return "EARLY";
    if (minutes <= ONTIME_LIMIT) return "ONTIME";
    return "LATE";
}

int main() {
    int h, m;
    cin >> h >> m;

    // 调用封装好的检查函数
    if (!isValidTime(h, m)) {
        cout << "Invalid\n";
        return 0;
    }

    int now = h * 60 + m;
    cout << getStatus(now) << '\n';
    return 0;
}
#endif

//若输入格式是HH:MM:
#include <iostream>
#include <string>
#include <cctype>
#include <utility>
using namespace std;

// ==================== 常量区 ====================
const int EARLY_LIMIT = 7 * 60 + 30;
const int ONTIME_LIMIT = 8 * 60;
const int MIN_HOUR = 0;
const int MAX_HOUR = 23;
const int MIN_MINUTE = 0;
const int MAX_MINUTE = 59;

// ==================== 合法性检查器（放在最前面，让后续函数可见） ====================
bool isValidTime(int h, int m) {
    bool hourValid = (h >= MIN_HOUR) && (h <= MAX_HOUR);
    bool minuteValid = (m >= MIN_MINUTE) && (m <= MAX_MINUTE);
    return hourValid && minuteValid;
}

// ==================== 核心业务逻辑 ====================
string getStatus(int minutes) {
    if (minutes < EARLY_LIMIT) return "EARLY";
    if (minutes <= ONTIME_LIMIT) return "ONTIME";
    return "LATE";
}

// ==================== 时间解析器（现在 isValidTime 已经先定义了，可以安全调用） ====================
pair<bool, int> parseTime(const string& timeStr) {
    // 1. 检查长度
    if (timeStr.size() != 5) return {false, 0};

    // 2. 检查冒号位置
    if (timeStr[2] != ':') return {false, 0};

    // 3. 检查字符是否为数字
    for (int i = 0; i < 5; ++i) {
        if (i == 2) continue;
        if (!isdigit(timeStr[i])) return {false, 0};
    }

    // 4. 提取小时和分钟
    int h = stoi(timeStr.substr(0, 2));
    int m = stoi(timeStr.substr(3, 2));

    // 5. 检查数值范围（现在 isValidTime 已经可见了！）
    if (!isValidTime(h, m)) return {false, 0};

    return {true, h * 60 + m};
}

// ==================== 主入口 ====================
int main() {
    string input;
    cin >> input;

    auto [ok, minutes] = parseTime(input);

    if (!ok) {
        cout << "Invalid\n";
        return 0;
    }

    cout << getStatus(minutes) << '\n';
    return 0;
}
