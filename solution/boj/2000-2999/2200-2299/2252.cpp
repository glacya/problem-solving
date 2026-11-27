// BOJ 2252 - 줄 세우기
// 난이도: Gold III
// 푼 날짜: 2023-09-06
// https://www.acmicpc.net/problem/2252

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

constexpr ll mod = 1e9 + 7;

vector<int> visited;
vector<unordered_set<int>> edges;
vector<vector<int>> nodes;
vector<int> indegree;

int node_count;

int simplify(int v) {
    int ret = -1;
    for (int c: edges[v]) {
        if (visited[c] == 0) {
            visited[c] = 1;
            
            ret = simplify(c);
        }
        else if (visited[c] == 1) {
            indegree[c]--;
            ret = c;
        }
    }
    visited[v] = 2;
    
    if (ret >= 0) {
        if (v != ret) {
            nodes[ret].push_back(v);
            indegree[v] = 10000000;
            node_count--;
            return ret;
        }
        else {
            edges[v].erase(nodes[v].back());
        }
    }
    return -1;
}

void collect(int v) {
    
}

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    visited = vector<int>(n + 1);
    edges = vector<unordered_set<int>>(n + 1);
    nodes = vector<vector<int>>(n + 1);
    indegree = vector<int>(n + 1);
    
    node_count = n;
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        
        edges[u].insert(v);
        indegree[v]++;
    }
    
    for (int i = 1; i <= n; i++) {
        if (visited[i] == 0) {
            visited[i] = 1;
            simplify(i);
        }
    }
    
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        // cout << indegree[i] << " ";
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        
        cout << v << " ";
        for (int x: nodes[v]) {
            cout << x << " ";
        }
        
        for (int c: edges[v]) {
            if (--indegree[c] == 0) {
                q.push(c);
            }
        }
    }
}
