// BOJ 1080 - 행렬
// 난이도: Silver I
// 푼 날짜: 2023-12-09
// https://www.acmicpc.net/problem/1080

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<bool>> start(n, vector<bool>(m));
    vector<vector<bool>> goal(n, vector<bool>(m));
    
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        
        for (int j = 0; j < m; j++) {
            start[i][j] = s[j] == '1';
        }
    }
    
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        
        for (int j = 0; j < m; j++) {
            goal[i][j] = s[j] == '1';
        }
    }
    
    if (n < 3 || m < 3) {
        if (start == goal) cout << 0;
        else cout << -1;
        
        return 0;
    }
    
    int cnt = 0;
    for (int i = 0; i < n - 2; i++) {
        for (int j = 0; j < m - 2; j++) {
            if (start[i][j] != goal[i][j]) {
                for (int x = 0; x < 3; x++) {
                    for (int y = 0; y < 3; y++) {
                        start[i + x][j + y] = !start[i + x][j + y];
                    }
                }
                
                cnt++;
            }
        }
    }
    
    if (start != goal) cout << -1;
    else cout << cnt;
}
