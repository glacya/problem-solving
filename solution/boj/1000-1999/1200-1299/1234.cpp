// BOJ 1234 - 크리스마스 트리
// 난이도: Gold II
// 푼 날짜: 2024-01-21
// https://www.acmicpc.net/problem/1234

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

ll comb(ll n, ll r) {
    ll v = 1;
    
    for (ll i = n - r + 1; i <= n; i++) {
        v *= i;
    }
    
    for (ll i = 1; i <= r; i++) {
        v /= i;
    }
    
    return v;
}

int main() {
    fastio;
    
    int n, r, g, b;
    cin >> n >> r >> g >> b;
    
    queue<tuple<int, int, int, int>> q;
    vector<vector<vector<ll>>> dp(101, vector<vector<ll>>(101, vector<ll>(101)));
    dp[r][g][b] = 1;
    
    q.push(make_tuple(r, g, b, 0));
    
    ll ans = 0;
    while (!q.empty()) {
        auto [red, green, blue, step] = q.front();
        q.pop();
        
        // cout << "On node " << red << ", " << green << ", " << blue << ". Step = " << step << "\n";
        
        if (step == n) {
            ans += dp[red][green][blue];
            continue;
        }
        
        vector<int> val = {red, green, blue};
        
        int next = step + 1;
        
        if (next % 2 == 0) {
            for (int i = 0; i < 3; i++) {
                bool ok = true;
                for (int j = 0; j < 3; j++) {
                    if (i != j) {
                        val[j] -= next / 2;
                        
                        if (val[j] < 0) ok = false;
                    }
                }
                
                if (ok) {
                    if (dp[val[0]][val[1]][val[2]] == 0) q.push(make_tuple(val[0], val[1], val[2], next));
                        
                    dp[val[0]][val[1]][val[2]] += dp[red][green][blue] * comb(next, next / 2);
                }
                
                for (int j = 0; j < 3; j++) {
                    if (i != j) {
                        val[j] += next / 2;
                    }
                }
            }
        }
        
        if (next % 3 == 0) {
            int nr = red - next / 3;
            int ng = green - next / 3;
            int nb = blue - next / 3;
            
            if (nr >= 0 && ng >= 0 && nb >= 0) {
                if (dp[nr][ng][nb] == 0) q.push(make_tuple(nr, ng, nb, next));
                
                dp[nr][ng][nb] += comb(next, next / 3) * comb(next * 2 / 3, next / 3) * dp[red][green][blue];
            }
        }
        
        for (int i = 0; i < 3; i++) {
            val[i] -= next;
            
            if (val[i] >= 0) {
                if (dp[val[0]][val[1]][val[2]] == 0) q.push(make_tuple(val[0], val[1], val[2], next));
                
                dp[val[0]][val[1]][val[2]] += dp[red][green][blue];
            }
            
            val[i] += next;
        }
    }
    
    cout << ans;
}
