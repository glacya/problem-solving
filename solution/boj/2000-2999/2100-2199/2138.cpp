// BOJ 2138 - 전구와 스위치
// 난이도: Gold IV
// 푼 날짜: 2023-12-09
// https://www.acmicpc.net/problem/2138

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
    
    string s, t;
    cin >> s >> t;
    
    int cnt = 1e9;
    for (int i = 0; i < 2; i++) {
        string x = s;
        
        int c = 0;
        if (i == 0) {
            x[0] = '0' + '1' - x[0];
            x[1] = '0' + '1' - x[1];
            
            c++;
        }
        
        for (int j = 1; j < n; j++) {
            if (x[j - 1] != t[j - 1]) {
                x[j - 1] = '0' + '1' - x[j - 1];
                x[j] = '0' + '1' - x[j];
                
                if (j + 1 < n)
                    x[j + 1] = '0' + '1' - x[j + 1];
                    
                c++;
            }
        }
        
        if (x == t)
            cnt = min(c, cnt);
    }
    
    cout << (cnt == 1e9 ? -1 : cnt);
}
