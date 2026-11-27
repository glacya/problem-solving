// BOJ 1766 - 문제집
// 난이도: Gold II
// 푼 날짜: 2023-10-09
// https://www.acmicpc.net/problem/1766

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> edges(n + 1);
    vector<int> indeg(n + 1);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        
        edges[u].push_back(v);
        indeg[v]++;
    }
    
    priority_queue<int, vector<int>, greater<int>> q;
    for (int i = 1; i <= n; i++) {
        if (indeg[i] == 0) q.push(i);
    }
    
    while (!q.empty()) {
        int v = q.top();
        q.pop();
        
        cout << v << " ";
        
        for (int c: edges[v]) {
            if (--indeg[c] == 0) {
                q.push(c);
            }
        }
    }
}
