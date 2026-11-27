// BOJ 2098 - 외판원 순회
// 난이도: Gold I
// 푼 날짜: 2023-08-31
// https://www.acmicpc.net/problem/2098

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

constexpr int X = 1e8;

vector<vector<int>> edges;
vector<vector<int>> dp;

// int dfscall = 0;
// int fs = 0;
void dfs(int vertex, int visited, int n) {
    // dfscall++;
    // cout << "dfs(" << vertex << ",\t" << bitset<32>(visited) << ")\n";
    if (dp[vertex][visited] != -1) {
        // cout << "\tDuplicate.\n";
        return;
    }
    
    if (visited == ((1 << n) - 1)) {
        if (edges[vertex][0]) {
            dp[vertex][visited] = edges[vertex][0];
        }
        else {
            dp[vertex][visited] = X;
        }
        return;
    }
    

    bool no_further = true;
    int bm = 1;    
    for (int i = 0; i < n; i++) {
        if (!(visited & bm) && edges[vertex][i]) {
            no_further = false;
            dfs(i, visited | bm, n);
            
            if (dp[vertex][visited] == -1)
                dp[vertex][visited] = dp[i][visited | bm] + edges[vertex][i];
            else
                dp[vertex][visited] = min(dp[vertex][visited], dp[i][visited | bm] + edges[vertex][i]);
        }
        bm <<= 1;
    }
    
    if (no_further) {
        // fs++;
        // cout << "  dfs(" << vertex << ",\t" << bitset<32>(visited) << ") has no way..\n";
        dp[vertex][visited] = X;
    }
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    edges = vector<vector<int>>(n, vector<int>(n));
    dp = vector<vector<int>>(n, vector<int>(1 << n, -1));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> edges[i][j];
        }
    }
    
    dfs(0, 1, n);
    cout << dp[0][1] << "\n";
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < (1 << n); j++) {
    //         cout << dp[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << dfscall << " " << fs;
    
    return 0;
}
