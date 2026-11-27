// BOJ 1305 - 광고
// 난이도: Platinum IV
// 푼 날짜: 2023-11-25
// https://www.acmicpc.net/problem/1305

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;

    int m;
    cin >> m;
    
    string p;
    cin >> p;
    
    vector<int> fail(m);
    
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && p[i] != p[j]) {
            j = fail[j - 1];
        }
        
        if (p[i] == p[j]) {
            fail[i] = ++j;
        }
    }
    
    cout << m - fail[m - 1];
}
