// BOJ 2109 - 순회강연
// 난이도: Gold III
// 푼 날짜: 2023-12-09
// https://www.acmicpc.net/problem/2109

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
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
    
    parent[p2] = p1;
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    parent = vector<int>(10001);
    
    for (int i = 1; i <= 10000; i++) {
        parent[i] = i;
    }
    
    vector<pair<int, int>> semi(n);
    
    for (int i = 0; i < n; i++) {
        int p, d;
        cin >> p >> d;
        
        semi[i] = {p, d};
    }
    
    sort(semi.begin(), semi.end(), greater<pair<int, int>>());
    
    int income = 0;
    for (int i = 0; i < n; i++) {
        auto [c, d] = semi[i];
        
        int p = find(d);
        if (p > 0) {
            income += c;
            unite(p - 1, p);
        }
    }
    
    cout << income;
}
