// BOJ 1562 - 계단 수
// 난이도: Gold I
// 푼 날짜: 2023-09-14
// https://www.acmicpc.net/problem/1562

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

constexpr ll mod = 1e9;

int main() {
    int n;
    cin >> n;
    
    vector<vector<ll>> dp(10, vector<ll>(1024));
    
    for (int i = 1; i <= 9; i++) {
        dp[i][1 << i] = 1;
    }
    
    for (int t = 1; t < n; t++) {
        vector<vector<ll>> next_dp(10, vector<ll>(1024));
        for (int i = 0; i < 10; i++) {
            for (int bm = 0; bm < 1024; bm++) {
                if (i > 0) {
                    int next_bm = bm | (1 << (i - 1));
                    next_dp[i - 1][next_bm] += dp[i][bm];
                    next_dp[i - 1][next_bm] %= mod;
                }
                
                if (i < 9) {
                    int next_bm = bm | (1 << (i + 1));
                    next_dp[i + 1][next_bm] += dp[i][bm];
                    next_dp[i + 1][next_bm] %= mod;
                }
            }
        }
        dp = next_dp;
    }
    
    ll ans = 0;
    for (int i = 0; i < 10; i++) {
        ans = (ans + dp[i][1023]) % mod;
    }
    cout << ans;
    
    return 0;
}
