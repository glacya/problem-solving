// BOJ 1244 - 스위치 켜고 끄기
// 난이도: Silver IV
// 푼 날짜: 2023-10-13
// https://www.acmicpc.net/problem/1244

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    vector<int> sw(n + 1);
    for (int i = 1; i <= n; i++) cin >> sw[i];
    
    int q;
    cin >> q;
    
    while (q--) {
        int g, u;
        cin >> g >> u;
        
        if (g == 1) {
            for (int i = 1; i * u <= n; i++) {
                sw[i * u] = 1 - sw[i * u];
            }
        }
        else {
            sw[u] = 1 - sw[u];
            int idx = 1;
            while (u - idx > 0 && u + idx <= n && sw[u - idx] == sw[u + idx]) {
                sw[u - idx] = 1 - sw[u - idx];
                sw[u + idx] = 1 - sw[u + idx];
                idx++;
            }
        }
    }
    
    for (int i = 1; i <= n; i++) {
        cout << sw[i] << " ";
        
        if (i % 20 == 0) cout << "\n";
    }
}
