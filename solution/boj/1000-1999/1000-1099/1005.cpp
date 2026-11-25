// BOJ 1005 - ACM Craft
// 난이도: Gold III
// 푼 날짜: 2023-06-28
// https://www.acmicpc.net/problem/1005

#include <iostream>
#include <vector>
#include <queue>

using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int t;
    cin >> t;
    
    for (int z = 0; z < t; z++) {
        int N, K;
        cin >> N >> K;
        
        vector<ll> duration(N + 1);
        vector<ll> in_deg(N + 1);
        
        for (int i = 1; i <= N; i++) {
            cin >> duration[i];
        }
        
        vector<vector<bool>> edges(N + 1, vector<bool>(N + 1, false));
        for (int i = 0; i < K; i++) {
            int X, Y;
            cin >> X >> Y;
            
            edges[X][Y] = true;
            in_deg[Y]++;
        }
        
        int W;
        cin >> W;
        
        vector<ll> vertices;
        queue<int> q;
        
        for (int i = 1; i <= N; i++) {
            if (in_deg[i] == 0) {
                q.push(i);
            }
        }
        
        for (int i = 1; i <= N; i++) {
            if (q.empty()) {
                // Not gonna happen in this problem.
                return 0;
            }
            
            int v = q.front();
            q.pop();
            
            vertices.push_back(v);
            for (int j = 1; j <= N; j++) {
                if (edges[v][j]) {
                    if (--in_deg[j] == 0) {
                        q.push(j);
                    }
                }
            }
        }
        
        vector<ll> dp(N + 1);
        for (int v: vertices) {
            if (dp[v] == 0) {
                dp[v] = duration[v];
            }
            
            if (v == W) break;
            
            for (int i = 1; i <= N; i++) {
                if (edges[v][i]) {
                    dp[i] = max(dp[i], dp[v] + duration[i]);
                }
            }
        }
        
        cout << dp[W] << "\n";
    }

    return 0;
}
