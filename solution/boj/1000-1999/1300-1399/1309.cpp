// BOJ 1309 - 동물원
// 난이도: Silver I
// 푼 날짜: 2023-12-08
// https://www.acmicpc.net/problem/1309

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int mod = 9901;
    
    int dp[2] = {1, 0};
    int n;
    cin >> n;
    
    for (int i = 0; i < n; i++) {
        int v0 = (dp[0] + dp[1]) % mod;
        int v1 = (dp[0] * 2 + dp[1]) % mod;
        
        dp[0] = v0;
        dp[1] = v1;
    }
    
    cout << (dp[0] + dp[1]) % mod;
}
