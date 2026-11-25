// BOJ 1068 - 트리
// 난이도: Gold V
// 푼 날짜: 2023-08-24
// https://www.acmicpc.net/problem/1068

#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

int dfs(vector<vector<int>> &edges, int vertex, int destroyed) {
    if (vertex == destroyed) return 0;
    
    int leaf = 0;
    
    for (int c: edges[vertex]) {
        if (c != destroyed) {
            leaf += dfs(edges, c, destroyed);
        }    
    }
    
    return leaf ? leaf : 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int n;
    cin >> n;
    
    vector<vector<int>> edges(n);
    
    int root = -1;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        
        if (x == -1) root = i;
        else {
            edges[x].push_back(i);
        }
    }
    
    int destroyed;
    cin >> destroyed;

    cout << dfs(edges, root, destroyed);

    return 0;
}
