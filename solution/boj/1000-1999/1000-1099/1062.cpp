// BOJ 1062 - 가르침
// 난이도: Gold IV
// 푼 날짜: 2023-12-08
// https://www.acmicpc.net/problem/1062

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, k;
    cin >> n >> k;
    
    if (k < 5) {
        cout << 0;
        return 0;
    }
    
    vector<int> words(n);
    
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        
        for (char c: s) {
            if (c == 'a' || c == 'c' || c == 'i' || c == 'n' || c == 't') continue;
            
            int x = (c - 'a') - 1;
            if (c > 'c') x--;
            if (c > 'i') x--;
            if (c > 'n') x--;
            if (c > 't') x--;
            words[i] |= (1 << x);
        }
    }
    
    int mc = 0;
    for (int i = 0; i < (1 << 21); i++) {
        if (__builtin_popcount(i) != k - 5) continue;
        
        int cnt = 0;
        for (int j = 0; j < n; j++) {
            if ((words[j] & i) == words[j]) {
                cnt++;
            }
        }
        
        mc = max(mc, cnt);
    }
    cout << mc;
}
