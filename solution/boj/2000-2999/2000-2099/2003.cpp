// BOJ 2003 - 수들의 합 2
// 난이도: Silver IV
// 푼 날짜: 2023-12-08
// https://www.acmicpc.net/problem/2003

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int n;
    ll m;
    cin >> n >> m;
    
    vector<ll> arr(n);
    
    for (int i = 0; i < n; i++) cin >> arr[i];
    
    int left = 0;
    int right = 0;
    ll total = 0;
    int cnt = 0;
    
    while (1) {
        if (total < m) {
            if (right == n) break;
            
            total += arr[right++];
        }
        else {
            if (total == m) cnt++;
            
            total -= arr[left++];
        }
    }
    
    cout << cnt;
}
