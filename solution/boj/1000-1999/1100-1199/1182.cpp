// BOJ 1182 - 부분수열의 합
// 난이도: Silver II
// 푼 날짜: 2023-12-09
// https://www.acmicpc.net/problem/1182

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, s;
    cin >> n >> s;
    
    vector<int> arr(n);
    
    for (int i = 0; i < n; i++) cin >> arr[i];
    
    int cnt = 0;
    for (int i = 1; i < (1 << n); i++) {
        int p = 0;
        
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) p += arr[j];
        }
        
        if (p == s) cnt++;
    }
    
    cout << cnt;
}
