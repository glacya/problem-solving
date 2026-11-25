// BOJ 1188 - 음식 평론가
// 난이도: Gold IV
// 푼 날짜: 2023-12-22
// https://www.acmicpc.net/problem/1188

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int rec(int n, int m) {
    n %= m;
    
    if (n == 0) return 0;
    else if (m % n == 0) return (n * (m / n - 1));
    else return n * (m / n) + rec(n, m % n);
}

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    cout << rec(n, m);
}
