// BOJ 1178 - 간선 추가
// 난이도: Platinum V
// 푼 날짜: 2023-10-16
// https://www.acmicpc.net/problem/1178

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

vector<int> parent;
vector<int> set_size;
vector<bool> visited;
vector<int> degree;

int find_parent(int v) {
    if (v == parent[v]) return v;
    else return parent[v] = find_parent(parent[v]);
}

void union_parent(int v1, int v2) {
    int p1 = find_parent(v1);
    int p2 = find_parent(v2);
    
    if (p1 > p2) swap(p1, p2);
    
    if (p1 != p2) {
        parent[p2] = p1;
        set_size[p1] = set_size[p1] + set_size[p2];
    }
}

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    parent = vector<int>(n + 1);
    set_size = vector<int>(n + 1, 1);
    visited = vector<bool>(n + 1);
    degree = vector<int>(n + 1);
    
    for (int i = 1; i <= n; i++) parent[i] = i;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        
        union_parent(u, v);
        degree[u]++;
        degree[v]++;
    }
    
    map<int, int> odd_degs;
    
    for (int i = 1; i <= n; i++) {
        if (degree[i] % 2) {
            odd_degs[find_parent(i)]++;
        }
        else {
            odd_degs[find_parent(i)];
        }
    }
    
    int edges = 0;
    int zd = 0;
    int td = 0;
    for (auto p: odd_degs) {
        if (p.second >= 2) {
            edges += (p.second - 2) / 2;
            td++;
        }
        else zd++;
    }
    
    cout << edges + (int) odd_degs.size() - 1;
}
