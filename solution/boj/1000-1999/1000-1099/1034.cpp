// BOJ 1034 - 램프
// 난이도: Gold IV
// 푼 날짜: 2023-10-11
// https://www.acmicpc.net/problem/1034

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    vector<pair<ll, int>> nums(n);
    
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        
        ll val = 0;
        for (int j = 0; j < m; j++) {
            if (s[j] == '1') {
                val |= (1LL << j);
            }
            else nums[i].second++;
        }
        nums[i].first = val;
    }
    
    int k;
    cin >> k;
    
    map<ll, int> freq;
    for (int i = 0; i < n; i++) {
        ll val = nums[i].first;
        int zeros = nums[i].second;
        
        if (zeros <= k && (k - zeros) % 2 == 0) {
            freq[val]++;
        }
    }
    
    int maxf = 0;
    for (auto p: freq) {
        maxf = max(maxf, p.second);
    }
    cout << maxf;
}
