// BOJ 2150 - Strongly Connected Component
// 난이도: Platinum V
// 푼 날짜: 2023-10-12
// https://www.acmicpc.net/problem/2150

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

vector<int> v_index;
vector<vector<int>> edges;
vector<bool> collected;
stack<int> st;
vector<vector<int>> scc;

int x = 0;

int dfs(int v) {
    v_index[v] = x++;
    st.push(v);
    
    // Record the highest reachable node's v_index.
    int reach_high = v_index[v];
    
    // Check children nodes. 
    for (int c: edges[v]) {
        if (v_index[c] < 0) {
            // Record how high the children can reach.
            reach_high = min(reach_high, dfs(c));
        }
        else if (!collected[c]) {
            reach_high = min(reach_high, v_index[c]);
        }
    }
    
    // The highest reachable node is v, meaning a SCC is found.
    if (reach_high == v_index[v]) {
        vector<int> new_scc;
        
        while (!st.empty()) {
            int k = st.top();
            st.pop();
            
            collected[k] = true;
            new_scc.push_back(k);
            
            if (k == v) {
                break;
            }
        }
        
        sort(new_scc.begin(), new_scc.end());
        scc.push_back(new_scc);
    }
    
    return reach_high;
}

int main() {
    fastio;
    
    int n, e;
    cin >> n >> e;
    
    edges = vector<vector<int>>(n + 1);
    v_index = vector<int>(n + 1, -1);
    collected = vector<bool>(n + 1);
    
    for (int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;
        
        edges[u].push_back(v);
    }
    
    for (int i = 1; i <= n; i++) {
        if (!collected[i]) {
            dfs(i);
        }
    }
    
    sort(scc.begin(), scc.end());
    
    cout << scc.size() << "\n";
    for (auto v: scc) {
        for (auto x: v) {
            cout << x << " ";
        }
        cout << "-1\n";
    }
}
