// BOJ 1914 - 하노이 탑
// 난이도: Gold V
// 푼 날짜: 2023-08-17
// https://www.acmicpc.net/problem/1914

#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef __int128 lll;

void hanoi(int n, int i1, int i2) {
    if (n == 0) return;
    
    int i3 = 1;
    
    for (int i = 1; i <= 3; i++) {
        if (i != i1 && i != i2) {
            i3 = i;
            break;
        }
    }
    
    hanoi(n - 1, i1, i3);
    cout << i1 << " " << i2 << "\n";
    hanoi(n - 1, i3, i2);
}

int main()
{
    int N;
    cin >> N;
    
    lll bn = (lll) 1;
    bn <<= N;
    bn -= 1;
    
    lll mid = (lll) 1e18;
    
    if (bn / mid > 0) {
        cout << (ll) (bn / mid);
    }
    
    cout << (ll) (bn % mid) << "\n";
    
    if (N <= 20)
        hanoi(N, 1, 3);

    return 0;
}
