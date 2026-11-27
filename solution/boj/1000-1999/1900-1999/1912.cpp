// BOJ 1912 - 연속합
// 난이도: Silver II
// 푼 날짜: 2023-05-23
// https://www.acmicpc.net/problem/1912

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
#define fio ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);

int main()
{
    fio;
    int n;
    cin >> n;
    
    int acc = 0;
    int maximum = -1e9;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        acc += x;    
    
        maximum = max(maximum, acc);
        if (acc < 0) acc = 0;
    }
    
    cout << maximum;
}
