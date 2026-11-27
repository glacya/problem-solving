// BOJ 2228 - 구간 나누기
// 난이도: Gold III
// 푼 날짜: 2023-07-15
// https://www.acmicpc.net/problem/2228

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int local_max(vector<int> &arr, int x, int y) {
    int loc = arr[x];
    int glob = arr[x];
    
    for (int i = x + 1; i <= y; i++) {
        loc = max(loc + arr[i], arr[i]);
        if (loc > glob) {
            glob = loc;
        }
    }
    
    return glob;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int N, M;
    cin >> N >> M;
    
    vector<int> array(N);
    vector<vector<int>> local_sum(N, vector<int>(N));
    vector<vector<int>> dp(N, vector<int>(M + 1, -1000000000));
    
    int global_maximum = -1000000000;
    
    for (int i = 0; i < N; i++) {
        cin >> array[i];
    }
    
    for (int i = N - 1; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            if (i + 2 < N) {
                for (int m = 1; m <= M; m++) {
                    dp[i - j][m] = max(dp[i - j][m], dp[i + 2][m - 1] + local_max(array, i - j, i));
                }
            }
            else {
                dp[i - j][1] = max(dp[i - j][1], local_max(array, i - j, i));
            }
        }
        global_maximum = max(global_maximum, dp[i][M]);
    }
    
    // for (auto v: dp) {
    //     for (auto x: v) {
    //         cout << x << " ";
    //     }
    //     cout << "\n";
    // }
    
    cout << global_maximum;
    
    return 0;
}
