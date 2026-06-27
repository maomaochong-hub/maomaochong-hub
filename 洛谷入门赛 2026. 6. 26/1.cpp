/*题目描述:
小明买了α个面包和b盒牛奶。
每个面包p元，每盒牛奶q元。
小明共支付m元。
请先输出小明应付的总价，再输出小明应得的找零。找零等于m－总价。
输入格式
一行5个整数：a,b,P,q,m
输出格式
输出共两行。第一行输出小明应付的总价，第二行输出小明应得的找零。
*/
#include<bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0); // 输入输出流
    int a, b, p, q, m;
    cin >> a >> b >> p >> q >> m;
    cout << a * p + b * q << endl << m - ( a * p + b * q) << endl; 

    return 0;
}
