// BOJ 1311 - 할 일 정하기 1
// 난이도: Gold I
// 푼 날짜: 2023-11-21
// https://www.acmicpc.net/problem/1311

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    vector<vector<int>> cost(n, vector<int>(n));
    vector<int> dp(1 << n, 1e9);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> cost[i][j];
        }
    }
    
    queue<int> q;
    q.push(0);
    dp[0] = 0;
    
    while (!q.empty()) {
        int bm = q.front();
        q.pop();
        
        // cout << "Considering bm: " << bitset<20>(bm) << "\n";
        
        int bits = __builtin_popcount(bm);
        
        for (int i = 0; i < n; i++) {
            if (!(bm & (1 << i))) {
                int nbm = bm | (1 << i);
                
                if (dp[nbm] > dp[bm] + cost[i][bits]) {
                    if (dp[nbm] == 1e9) q.push(nbm);
                    
                    dp[nbm] = dp[bm] + cost[i][bits];
                }
            }
        }
    }
    
    cout << dp[(1 << n) - 1];
}
