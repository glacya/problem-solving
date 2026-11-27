// BOJ 1334 - 다음 팰린드롬 수
// 난이도: Gold V
// 푼 날짜: 2023-10-10
// https://www.acmicpc.net/problem/1334

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    string s;
    cin >> s;
    
    int n = s.size();
    bool lower_ge = true;
    
    for (int i = n / 2 - 1; i >= 0; i--) {
        int j = n - 1 - i;
        
        if (s[j] != s[i]) {
            lower_ge = s[j] > s[i];
            break;
        }
    }
    
    if (lower_ge) {
        s[(n - 1) / 2]++;
        for (int i = (n - 1) / 2; i >= 0; i--) {
            if (s[i] > '9') {
                s[i] = '0';
                if (i == 0) {
                    s = string("1") + s;
                }
                else {
                    s[i - 1]++;
                }
            }
        }
        
        for (int i = (n + 1) / 2; i < s.size(); i++) {
            s[i] = s[s.size() - 1 - i];
        }
        
        cout << s;
    }
    else {
        string f = s.substr(0, n / 2);
        
        cout << f;
        if (n % 2) cout << s[n / 2];
        reverse(f.begin(), f.end());
        cout << f;
    }
}
