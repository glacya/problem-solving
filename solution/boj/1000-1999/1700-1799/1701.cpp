// BOJ 1701 - Cubeditor
// 난이도: Gold II
// 푼 날짜: 2023-11-25
// https://www.acmicpc.net/problem/1701

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    string s;
    cin >> s;
    string_view sp(s);
    
    int m = sp.size();
    int glmax = 0;
    
    for (int t = 0; t < m; t++) {
        string_view p = sp.substr(t, m);
        int n = p.size();
        vector<int> fail(n);
        for (int i = 1, j = 0; i < n; i++) {
            while (j > 0 && p[i] != p[j]) {
                j = fail[j - 1];
            }
            
            if (p[i] == p[j]) {
                fail[i] = ++j;
                glmax = max(glmax, fail[i]);
            }
        }
    }
    
    cout << glmax;
}
