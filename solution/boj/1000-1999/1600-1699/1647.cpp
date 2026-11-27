// BOJ 1647 - 도시 분할 계획
// 난이도: Gold IV
// 푼 날짜: 2023-09-04
// https://www.acmicpc.net/problem/1647

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

class Edge {
public:
    int u;
    int v;
    int w;
    
    Edge() {}
    Edge(int u, int v, int w) {
        this->u = u;
        this->v = v;
        this->w = w;
    }
    
    bool operator<(const Edge &e) const {
        return this->w > e.w;
    }
};

vector<int> parent;

int find_parent(int v) {
    if (v == parent[v]) return v;
    else return parent[v] = find_parent(parent[v]);
}

void union_parent(int v1, int v2) {
    int p1 = find_parent(v1);
    int p2 = find_parent(v2);
    
    if (p1 > p2) swap(p1, p2);
    parent[p1] = p2;
}

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    parent = vector<int>(n + 1);
    
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }
    
    priority_queue<Edge> edges;
    
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push(Edge(u, v, w));
    }
    
    int ws = 0;
    int count = 0;
    while (count < n - 2) {
        Edge e = edges.top();
        edges.pop();
        
        int p1 = find_parent(e.u);
        int p2 = find_parent(e.v);
        
        // cout << e.u << " " << e.v << " " << e.w << "\n";
        
        if (p1 == p2) continue;
        
        count++;
        ws += e.w;
        union_parent(p1, p2);
    }
    
    cout << ws;
}
