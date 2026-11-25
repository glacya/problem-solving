// BOJ 1272 - 특별 노드
// 난이도: Gold II
// 푼 날짜: 2023-10-11
// https://www.acmicpc.net/problem/1272

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

vector<ll> w;
vector<vector<ll>> edges;
vector<vector<ll>> dp;

void dfs(int v, int parent, int li) {
    if (dp[v][li] < 1e12) return;
    
    // cout << "dfs(" << v << ", " << li << ")\n";
    
    ll weight = w[v];
    
    if (v != li) weight -= w[li];
    
    for (int c: edges[v]) {
        if (c != parent) {
            dfs(c, v, li);
            dfs(c, v, c);
            
            weight += min(dp[c][li], dp[c][c]);
        }
    }
    
    dp[v][li] = weight;
    // cout << "dp[" << v << "][" << li << "] = " << weight << "\n";
}

int main() {
    fastio;
    
    int n, r;
    cin >> n >> r;
    
    w = vector<ll>(n + 1);
    edges = vector<vector<ll>>(n + 1);
    dp = vector<vector<ll>>(n + 1, vector<ll>(n + 1, 1e12));
    
    for (int i = 1; i <= n; i++) {
        cin >> w[i];
    }
    
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        
        edges[u].push_back(v);
        edges[v].push_back(u);
    }
    
    dfs(r, -1, r);
    
    cout << dp[r][r];
}
