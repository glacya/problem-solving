// BOJ 1948 - 임계경로
// 난이도: Platinum V
// 푼 날짜: 2023-09-20
// https://www.acmicpc.net/problem/1948

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    vector<int> dp(n + 1);
    vector<queue<int>> prevs(n + 1);
    vector<vector<pair<int, int>>> edges(n + 1);
    vector<int> indegree(n + 1);
    
    for (int i = 0; i < m; i++) {
        int u, v, x;
        cin >> u >> v >> x;
        
        edges[u].push_back(make_pair(v, x));
        indegree[v]++;
    }
    
    int s, d;
    cin >> s >> d;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        
        for (auto p: edges[v]) {
            int c = p.first;
            int w = p.second;
            
            int dist = dp[v] + w;
            if (dp[c] < dist) {
                dp[c] = dist;
                
                while (!prevs[c].empty()) prevs[c].pop();
                prevs[c].push(v);
            }
            else if (dp[c] == dist) {
                prevs[c].push(v);
            }
            
            if (--indegree[c] == 0) {
                q.push(c);
            }
        }
    }
    
    int roads = 0;
    queue<int> rev_track;
    vector<bool> visited(n + 1);
    rev_track.push(d);
    
    while (!rev_track.empty()) {
        int v = rev_track.front();
        rev_track.pop();
        
        if (visited[v]) continue;
        
        visited[v] = true;
        roads += (int) prevs[v].size();
        
        while (!prevs[v].empty()) {
            rev_track.push(prevs[v].front());
            prevs[v].pop();
        }
    }
    
    cout << dp[d] << "\n" << roads;
    
    return 0;
}
