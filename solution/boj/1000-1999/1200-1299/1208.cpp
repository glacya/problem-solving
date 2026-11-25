// BOJ 1208 - 부분수열의 합 2
// 난이도: Gold I
// 푼 날짜: 2023-08-28
// https://www.acmicpc.net/problem/1208

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

vector<ll> stuffs;
unordered_map<ll, int> weights;

int wx = 0;
ll case_total = 0;

void collect_weight(ll ws, int index, int n) {
    if (index == n) {
        weights[ws]++;
        return;
    }
    
    collect_weight(ws + stuffs[index], index + 1, n);
    collect_weight(ws, index + 1, n);
}

void compare_weight(ll ws, int index, int n, ll c) {
    if (index == n) {
        ll sum_left = c - ws;
        case_total += weights[sum_left];
        
        return;
    }
    
    compare_weight(ws + stuffs[index], index + 1, n, c);
    compare_weight(ws, index + 1, n, c);
}

int main() {
    fastio;
    
    int n;
    ll c;
    cin >> n >> c;
    
    int wl = 1 << (n / 2);
    
    stuffs = vector<ll>(n);
    
    for (int i = 0; i < n; i++) {
        cin >> stuffs[i];
    }
    
    collect_weight(0, 0, n / 2);
    
    compare_weight(0, n / 2, n, c);
    
    if (c == 0) case_total--;
    cout << case_total;
    
    return 0;
}
