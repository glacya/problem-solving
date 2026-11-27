// BOJ 2021 - 최소 환승 경로
// 난이도: Gold II
// 푼 날짜: 2023-10-08
// https://www.acmicpc.net/problem/2021

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, l;
    cin >> n >> l;
    
    vector<bool> set_visited(l);
    vector<bool> ver_visited(n + 1);
    vector<vector<int>> in_sets(n + 1);
    vector<vector<int>> set_info(l);
    
    for (int i = 0; i < l; i++) {
        while (1) {
            int x;
            cin >> x;
            
            if (x == -1) break;
            
            in_sets[x].push_back(i);
            set_info[i].push_back(x);
        }
    }
    
    int s, d;
    cin >> s >> d;
    
    if (s == d) {
        cout << "0";
        return 0;
    }
    
    vector<int> dist(n + 1, 1e7);
    queue<int> q;
    dist[s] = -1;
    q.push(s);
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        
        for (int se: in_sets[v]) {
            if (!set_visited[se]) {
                set_visited[se] = true;
                for (int x: set_info[se]) {
                    if (!ver_visited[x]) {
                        ver_visited[x] = true;
                        q.push(x);
                        dist[x] = min(dist[x], dist[v] + 1);
                    }
                }
            }
        }
    }
    
    cout << (dist[d] >= 1e7 ? -1 : dist[d]);
}
