// BOJ 2281 - 데스노트
// 난이도: Gold II
// 푼 날짜: 2023-07-20
// https://www.acmicpc.net/problem/2281

#include <iostream>
#include <vector>

using namespace std;

int sq(int x) {
    return x * x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> names(n);
    
    for (int i = 0; i < n; i++) {
        cin >> names[i];
    }
    
    int large = 1e9;
    vector<vector<int>> dp(n, vector<int>(m, large));
    dp[0][names[0] - 1] = 0;
    
    // for (int x: dp[0]) {
    //     cout << x << " ";
    // }
    // cout << "\n";
    
    for (int i = 1; i < n; i++) {
        for (int j = names[i] + 1; j < m; j++) {
            dp[i][j] = dp[i - 1][j - names[i] - 1];
        }
        
        int prev_min = large;
        for (int j = 0; j < m; j++) {
            prev_min = min(prev_min, dp[i - 1][j] + sq(m - 1 - j));
        }
        dp[i][names[i] - 1] = prev_min;
        
        // for (int x: dp[i]) {
        //     cout << x << " ";
        // }
        // cout << "\n";
    }
    
    int global_min = large;
    int step = n - 1;
    int index = names[step] - 1;
    while (index < m) {
        global_min = min(global_min, dp[n - 1][index]);
        index += names[--step] + 1;
    }
    cout << global_min;

    return 0;
}
