// BOJ 1922 - 네트워크 연결
// 난이도: Gold IV
// 푼 날짜: 2023-12-09
// https://www.acmicpc.net/problem/1922

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
    
    int n, m;
    cin >> n >> m;
    
    parent = vector<int>(n + 1);
    
    for (int i = 1; i <= n; i++) parent[i] = i;
    
    vector<tuple<int, int, int>> cables(m);
    
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        
        cables[i] = make_tuple(w, u, v);
    }
    
    sort(cables.begin(), cables.end());
    
    int k = 1;
    int mst = 0;
    int i = 0;
    while (k < n) {
        int w, u, v;
        tie(w, u, v) = cables[i++];
        
        if (find(u) != find(v)) {
            mst += w;
            unite(u, v);
            k++;
        }
    }
    cout << mst;
}
