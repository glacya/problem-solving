// BOJ 1761 - 정점들의 거리
// 난이도: Platinum V
// 푼 날짜: 2023-08-30
// https://www.acmicpc.net/problem/1761

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

constexpr int K = 16;

class Edge {
public:
    int v;
    int cost;
    
    Edge(int v, int c) {
        this->v = v;
        this->cost = c;
    }
};

vector<int> depth;
vector<vector<int>> parent;
vector<vector<Edge>> edges;
vector<bool> visited;
vector<int> costs;

int get_lca(int u, int v) {
    if (depth[u] < depth[v]) {
        swap(u, v);
    }
    
    int depth_diff = depth[u] - depth[v];
    for (int i = 0; depth_diff; i++) {
        if (depth_diff % 2) {
            u = parent[u][i];
        }
        depth_diff /= 2;
    }
    
    if (u != v) {
        for (int i = K - 1; i >= 0; i--) {
            if (parent[u][i] != -1 && parent[u][i] != parent[v][i]) {
                u = parent[u][i];
                v = parent[v][i];
            }
        }
        
        u = parent[u][0];
    }
    
    return u;
}

void traverse_tree(int vertex) {
    for (Edge e: edges[vertex]) {
        if (!visited[e.v]) {
            visited[e.v] = true;
            costs[e.v] = costs[vertex] + e.cost;
            parent[e.v][0] = vertex;
            depth[e.v] = depth[vertex] + 1;
            
            traverse_tree(e.v);
        }
    }
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    depth = vector<int>(n + 1);
    parent = vector<vector<int>>(n + 1, vector<int>(K, -1));
    edges = vector<vector<Edge>>(n + 1);
    visited = vector<bool>(n + 1);
    costs = vector<int>(n + 1);
    
    for (int x = 0; x < n - 1; x++) {
        int u, v, c;
        cin >> u >> v >> c;
        
        edges[u].push_back(Edge(v, c));
        edges[v].push_back(Edge(u, c));
    }
    
    visited[1] = true;
    traverse_tree(1);
    
    for (int j = 0; j < K - 1; j++) {
        for (int i = 1; i <= n; i++) {
            if (parent[i][j] != -1)
                parent[i][j + 1] = parent[parent[i][j]][j];
        }
    }
    
    int m;
    cin >> m;
    
    while (m--) {
        int u, v;
        cin >> u >> v;
        
        int lca = get_lca(u, v);
        int c1 = costs[u] - costs[lca];
        int c2 = costs[v] - costs[lca];
        
        cout << c1 + c2 << "\n";
    }
    
    return 0;
}
