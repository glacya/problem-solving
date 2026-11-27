// BOJ 2092 - 집합의 개수
// 난이도: Gold II
// 푼 날짜: 2023-12-26
// https://www.acmicpc.net/problem/2092

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int t, a, s, b;
    cin >> t >> a >> s >> b;
    
    vector<ll> num(t + 1);
    
    for (int i = 0; i < a; i++) {
        int x;
        cin >> x;
        
        num[x]++;
    }
    ll mod = 1000000;
    vector<ll> dp(a + 1);
    dp[0] = 1;
    
    for (int i = 1; i <= t; i++) {
        vector<ll> ndp(a + 1);
        
        for (int j = 0; j <= a; j++) {
            for (int k = 0; k <= num[i]; k++) {
                if (j + k <= a) {
                    ndp[j + k] = (ndp[j + k] + dp[j]) % mod;
                }
            }
        }
        
        dp = ndp;
    }
    
    ll ans = 0;
    for (int i = s; i <= b; i++) {
        ans = (ans + dp[i]) % mod;
    }
    cout << ans;
}
