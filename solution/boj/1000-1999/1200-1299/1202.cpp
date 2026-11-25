// BOJ 1202 - 보석 도둑
// 난이도: Gold II
// 푼 날짜: 2023-11-09
// https://www.acmicpc.net/problem/1202

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

vector<int> parent;

int find(int v) {
    if (v == parent[v]) return v;
    else return parent[v] = find(parent[v]);
}

void unite(int v1, int v2) {
    int p1 = find(v1);
    int p2 = find(v2);
    
    if (p1 > p2) swap(p1, p2);
    
    parent[p1] = p2;
}

int main() {
    fastio;
    
    int n, k;
    cin >> n >> k;
    
    vector<pair<ll, ll>> jewel(n);
    vector<ll> sack(k);

    parent = vector<int>(k + 1, k);
    
    for (int i = 0; i < n; i++) {
        ll m, v;
        cin >> m >> v;
        
        jewel[i] = {v, m};
    }
    
    for (int i = 0; i < k; i++) {
        cin >> sack[i];
        
        parent[i] = i;
    }
    
    sort(sack.begin(), sack.end());
    sort(jewel.begin(), jewel.end(), greater<pair<ll, ll>>());
    
    ll total_value = 0;
    
    for (int i = 0; i < n; i++) {
        ll value = jewel[i].first;
        ll weight = jewel[i].second;
        
        auto it = lower_bound(sack.begin(), sack.end(), weight);
        int dist = distance(sack.begin(), it);
        
        if (weight > *it) continue;
        
        int p = find(dist);
        
        if (p < k) {
            total_value += value;
            unite(p, p + 1);
        }
    }
    
    cout << total_value;
}
