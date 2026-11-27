// BOJ 1420 - 학교 가지마!
// 난이도: Platinum II
// 푼 날짜: 2023-12-17
// https://www.acmicpc.net/problem/1420

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

class Edge {
public: 
    int v;
    int capacity;
    int flow;
    Edge *rev;
    
    Edge() {}
    Edge(int v1, int c): v(v1), capacity(c), flow(0), rev(NULL) {}
};

vector<vector<Edge *>> edges;
vector<int> prev_v;
vector<string> world;

void dfs(int v, int sink) {
    if (prev_v[sink] > 0) return;
    
    for (Edge *e: edges[v]) {
        if (prev_v[e->v] < 0 && e->capacity - e->flow > 0) {
            prev_v[e->v] = v;
            dfs(e->v, sink);
        }
    }
}

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    edges = vector<vector<Edge *>>(n * m * 2);
    world = vector<string>(n);
    
    for (int i = 0; i < n; i++) {
        cin >> world[i];
    }
    
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    
    int source = -1, sink = -1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (world[i][j] == '#') continue;
            
            if (world[i][j] == 'K') source = i * m + j;
            else if (world[i][j] == 'H') sink = i * m + j;
            
            for (int t = 0; t < 4; t++) {
                int x = i + dx[t];
                int y = j + dy[t];
                
                if (x >= 0 && x < n && y >= 0 && y < m && world[x][y] != '#') {
                    int u = i * m + j;
                    int v = x * m + y;
                    
                    Edge *e1 = new Edge(v, 1);
                    Edge *e2 = new Edge(u + n * m, 0);
                    e1->rev = e2;
                    e2->rev = e1;
                    edges[u + n * m].push_back(e1);
                    edges[v].push_back(e2);
                }
            }
            
            int u = i * m + j;
            Edge *es1 = new Edge(u + n * m, 1);
            Edge *es2 = new Edge(u, 0);
            es1->rev = es2;
            es2->rev = es1;
            edges[u].push_back(es1);
            edges[u + n * m].push_back(es2);
        }
    }
    
    if (abs(source - sink) == m || (source / m == sink / m && abs(source - sink) == 1)) {
        cout << -1;
        return 0;
    }
    
    source += n * m;
    
    int max_flow = 0;
    while (1) {
        prev_v = vector<int>(n * m * 2, -1);
        prev_v[source] = source;
        dfs(source, sink);
        
        if (prev_v[sink] < 0) break;
        
        int flow = 1e9;
        int v = sink;
        while (v != source) {
            for (Edge *e: edges[v]) {
                if (e->v == prev_v[v]) {
                    flow = min(flow, e->rev->capacity - e->rev->flow);
                    break;
                }
            }
            
            v = prev_v[v];
        }
        
        v = sink;
        while (v != source) {
            for (Edge *e: edges[v]) {
                if (e->v == prev_v[v]) {
                    e->rev->flow += flow;
                    e->flow -= flow;
                    break;
                }
            }
            
            v = prev_v[v];
        }
        
        max_flow += flow;
    }
    
    assert(max_flow <= 4);
    cout << max_flow;
    
    for (int i = 0; i < n * m * 2; i++) {
        for (Edge *e: edges[i]) {
            // if (e->v == i + n * m)
            // cout << "(" << i << ", " << e->v << "): capacity = " << e->capacity << ", flow = " << e->flow << "\n";
            
            delete e;
        }
    }
}
