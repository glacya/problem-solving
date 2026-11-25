// BOJ 1014 - 컨닝
// 난이도: Platinum IV
// 푼 날짜: 2023-12-05
// https://www.acmicpc.net/problem/1014

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int t;
    cin >> t;
    
    while (t--) {
        int n, m;
        cin >> n >> m;
        
        vector<int> room(n);
        
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            
            for (int j = 0; j < m; j++) {
                room[i] <<= 1;
                if (s[j] == '.') {
                    room[i] += 1;
                }
            }
        }
        
        vector<int> dp(1 << m);
        
        int mv = 0;
        for (int r = 0; r < n; r++) {
            vector<int> ndp(1 << m);
            
            // cout << "ROW " << r << "\n";
            for (int bm = 0; bm < (1 << m); bm++) {
                int fixed = (1 << m) - 1;
                
                if (~(room[r] | ~bm)) {
                    continue;
                }
                
                int bc = 0;
                bool succ = true;
                int prev = 0;
                for (int j = 0; j < m; j++) {
                    if (bm & (1 << j)) {
                        if (prev == 1) {
                            succ = false;
                            break;
                        }
                        
                        if (j > 0) {
                            fixed &= ~(1 << (j - 1));
                        }
                        
                        if (j < m - 1) {
                            fixed &= ~(1 << (j + 1));
                        }
                        
                        bc++;
                        prev = 1;
                    }
                    else prev = 0;
                }
                if (!succ) continue;
                
                // cout << "Fixed value for " << bitset<10>(bm) << " = " << bitset<10>(fixed) << "\n";
                
                for (int pbm = 0; pbm < (1 << m); pbm++) {
                    if (~(fixed | ~pbm)) continue;
                    
                    // cout << "using entry " << bitset<10>(pbm) << ", ";
                    // cout << "updating ndp[" << bitset<10>(bm) << "]: " << dp[pbm] + bc << "\n";
                    ndp[bm] = max(ndp[bm], dp[pbm] + bc);
                }
                
                mv = max(mv, ndp[bm]);
            }
            // cout << "\n";
            
            dp = ndp;
        }
        
        cout << mv << "\n";
    }
}
