// BOJ 1327 - 소트 게임
// 난이도: Gold IV
// 푼 날짜: 2023-09-18
// https://www.acmicpc.net/problem/1327

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

int main() {
    fastio;
    
    int n, k;
    cin >> n >> k;
    
    string ins;
    string end;
    for (int i = 0; i < n; i++) {
        char c;
        cin >> c;
        
        ins += c;
        end += (char) i + '1';
    }
    
    map<string, int> dist;
    queue<pair<string, int>> q;
    
    q.push(make_pair(ins, 0));
    dist[ins] = 0;
    
    while (!q.empty()) {
        auto p = q.front();
        q.pop();
        
        string s = p.first;
        int v = p.second;
        
        if (s == end) break;
        
        for (int idx = 0; idx <= n - k; idx++) {
            string next = s;
            // cout << "Swapping " << next << " from " << idx << " to " << idx + k - 1 << "\n";
            for (int i = idx; i < idx + k / 2; i++) {
                swap(next[i], next[2 * idx + k - i - 1]);
            }
            // cout << "..result was " << next << "\n";
            
            if (dist.find(next) == dist.end()) {
                dist[next] = v + 1;
                q.push(make_pair(next, v + 1));
            }
        }
    }
    
    if (dist.find(end) == dist.end()) {
        cout << "-1";
    }
    else {
        cout << dist[end];
    }
    
    return 0;
}
