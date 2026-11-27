// BOJ 1450 - 냅색문제
// 난이도: Gold I
// 푼 날짜: 2023-08-28
// https://www.acmicpc.net/problem/1450

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

vector<ll> stuffs;
vector<ll> weights;

int wx = 0;
int case_total = 0;

void collect_weight(ll ws, int index, int n) {
    if (index == n) {
        weights[wx++] = ws;
        return;
    }
    
    collect_weight(ws + stuffs[index], index + 1, n);
    collect_weight(ws, index + 1, n);
}

void compare_weight(ll ws, int index, int n, ll c) {
    if (index == n) {
        ll sum_left = c - ws;
        auto it = upper_bound(weights.begin(), weights.end(), sum_left);
        int dist = distance(weights.begin(), it);
        case_total += dist;
        
        // cout << "Looking for " << sum_left << "\n";
        // for (int x: weights) {
        //     cout << x << " ";
        // }
        // cout << "\n";
        // cout << "Distance = " << dist << "\n";
        
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
    weights = vector<ll>(wl);
    
    for (int i = 0; i < n; i++) {
        cin >> stuffs[i];
    }
    
    collect_weight(0, 0, n / 2);
    sort(weights.begin(), weights.end());
    
    compare_weight(0, n / 2, n, c);
    cout << case_total;
    
    return 0;
}
