#if 0
题目描述:
播放器当前音量为v 接下来会按顺序按下n次按钮。第i次按下的按钮值记为ai（ai为0或1)
含义如下：
当ai=1时，表示按下”音量加”按钮：如果当前音量小于10，则当前音量增加1，否则当前音量不变
当ai=0时，表示按下”音量减”按钮：如果当前音量大于0，则当前音量减少1，否则当前音量不变
请输出全部操作结束后的音量。

输入格式
第一行两个整数n v
接下来n行，每行一个整数ai，其中每个ai都是0或1

输出格式
一行一个整数，表示最终音量

对于100%的数据，保证：1 ≤ n ≤ 10000，0 ≤ v ≤ 10，每个ai只能是0或1
#endif

#if 0
我的代码:
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n, v;
    cin >> n >> v;
    vector<int> V(n);
    for(int i = 0; i < n; i++)
    {
        cin >> V[i];
        if(v < 10 && V[i] == 1) v++;
        if(v > 0 && V[i] == 0) v--;
        if(v == 10 || v == 0) continue;
    }
    cout << v << endl;
    return 0;

问题:
存储多余：vector<int> V(n) 存放了所有操作，但从未用于后续处理，完全浪费内存（n ≤ 10000 虽小，但这是坏习惯）

冗余的 continue：if(v == 10 || v == 0) continue; 在循环末尾毫无意义，因为循环体已执行完毕

没有使用 ios::sync_with_stdio(false) 加速（虽然 n 不大，但养成好习惯）
#endif

#if 0
// 优化后的代码:
#include<iostream>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int n, v;
    cin >> n >> v;

    while(n --)
    {
        int OPTION;
        cin >> OPTION;

        if(OPTION == 1 && v < 10) v++;
        if(OPTION == 0 && v > 0) v--;
    }


    cout << v << endl;
    return 0;
}
#endif

//最终优化: 封装函数 + 支持调整上下限 + 合法性判断
#include <iostream>   // 用于 cin/cout
#include <vector>     // 用于存储操作序列
using namespace std;

// 音量上下限常量（防止魔数）
const int DEFAULT_MIN_VOL = 0;
const int DEFAULT_MAX_VOL = 10;
const int DEFAULT_UNIT_CHANGE = 1;

/**
 * 模拟音量调整过程
 * @param initVol  初始音量
 * @param ops      操作序列（0 = 减，1 = 加）
 * @param minVol   音量下限
 * @param maxVol   音量上限
 * @return 最终音量
 */
int simulateVolume(int initVol, const vector<int>& ops, int minVol, int maxVol) {
    int vol = initVol;              // 当前音量副本

    for (int op : ops) {            // 遍历每一个操作
        // 内部防御：如果操作值非法，忽略（但输入已确保合法）
        if (op != 0 && op != 1) {
            continue;
        }

        if (op == 1 && vol < maxVol) {
            vol += DEFAULT_UNIT_CHANGE;                  // 加音量
        } else if (op == 0 && vol > minVol) {
            vol -= DEFAULT_UNIT_CHANGE;                  // 减音量
        }
        // 如果已达边界，操作无效，不改变 vol
    }

    return vol;
}
//(*^▽^*) |ू･ω･` ) ┗( ▔, ▔ )┛ (✪ω✪) |ू･ω･` )
int main() {
    ios::sync_with_stdio(false);   
    cin.tie(nullptr);              

    int n, v;
    cin >> n >> v;                 

    vector<int> ops(n);            

    // ----------------- 输入合法性检查（外部） -----------------
    for (int i = 0; i < n; ++i) {
        cin >> ops[i];

        // 如果操作不是 0 或 1，说明输入非法
        if (ops[i] != 0 && ops[i] != 1) {
            cerr << "Error: invalid operation " << ops[i] << " at position " << i + 1 << "\n";
            return 1;              // 非正常退出
        }
    }

    // 调用核心函数，传入上下限常量（避免魔数）
    int finalVol = simulateVolume(v, ops, DEFAULT_MIN_VOL, DEFAULT_MAX_VOL);

    cout << finalVol << '\n';      // 输出最终音量

    return 0;
}
