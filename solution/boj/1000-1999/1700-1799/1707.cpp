// BOJ 1707 - 이분 그래프
// 난이도: Gold IV
// 푼 날짜: 2023-07-17
// https://www.acmicpc.net/problem/1707

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int T;
    cin >> T;
    
    while (T--) {
        int V, E;
        cin >> V >> E;
        
        vector<int> vertex_group(V + 1);
        vector<vector<int>> edges(V + 1);
        
        for (int i = 0; i < E; i++) {
            int u, v;
            cin >> u >> v;
            
            edges[u].push_back(v);
            edges[v].push_back(u);
        }
        
        bool wrong = false;
        
        queue<pair<int, int>> q;
        
        int cnt = 0;
        int index = 1;
        while (cnt < V && !wrong) {
            while (index <= V && vertex_group[index] != 0) {
                index++;
            }
            q.push(make_pair(index, 1));
            vertex_group[index] = 1;
            
            while (!q.empty() && !wrong) {
                auto p = q.front();
                q.pop();
                
                cnt++;
                int v = p.first;
                int kind = p.second;
                
                for (int neigh: edges[v]) {
                    if (vertex_group[neigh] == kind) {
                        wrong = true;
                        break;
                    }
                    else if (vertex_group[neigh] == 0) {
                        int next_kind = (kind == 1 ? 2 : 1);
                        vertex_group[neigh] = next_kind;
                        q.push(make_pair(neigh, next_kind));
                    }
                }
            }
        }
        cout << (wrong ? "NO\n" : "YES\n");
    }
    

    return 0;
}
