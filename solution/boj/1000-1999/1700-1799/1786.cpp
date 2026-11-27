// BOJ 1786 - 찾기
// 난이도: Platinum V
// 푼 날짜: 2023-11-24
// https://www.acmicpc.net/problem/1786

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    string s, p;
    getline(cin, s);
    getline(cin, p);
    
    int n = s.size();
    int m = p.size();
    
    vector<int> fail(m);
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && p[i] != p[j]) {
            j = fail[j - 1];
        }
        
        if (p[i] == p[j]) {
            fail[i] = ++j;
        }
    }
    
    vector<int> pos;
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && s[i] != p[j]) {
            j = fail[j - 1];
        }
        
        if (s[i] == p[j]) {
            j++;
            
            if (j == m) {
                pos.push_back(i - m + 1);
                j = fail[j - 1];
            }
        }
    }
    
    cout << pos.size() << "\n";
    for (int x: pos) {
        cout << x + 1 << " ";
    }
}
