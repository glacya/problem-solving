// BOJ 1285 - 동전 뒤집기
// 난이도: Gold I
// 푼 날짜: 2023-12-09
// https://www.acmicpc.net/problem/1285

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    vector<vector<bool>> coins(n, vector<bool>(n));
    vector<int> count(n);
    
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        
        for (int j = 0; j < n; j++) {
            coins[i][j] = (s[j] == 'T');
            if (s[j] == 'T') count[i]++;
        }
    }
    
    int min_coin = n * n;
    for (int i = 0; i < (1 << n); i++) {
        vector<int> new_count = count;
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {
                for (int k = 0; k < n; k++) {
                    if (coins[k][j]) {
                        new_count[k]--;
                    }
                    else {
                        new_count[k]++;
                    }
                }
            }
        }
        
        int c = 0;
        for (int j = 0; j < n; j++) {
            c += min(new_count[j], n - new_count[j]);
        }
        
        min_coin = min(min_coin, c);
    }
    
    cout << min_coin;
}
