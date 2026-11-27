// BOJ 2293 - 동전 1
// 난이도: Gold IV
// 푼 날짜: 2023-05-23
// https://www.acmicpc.net/problem/2293

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int n, k;
    cin >> n >> k;
    
    vector<int> dp(k + 1);
    vector<int> coins(n);
    
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }
    sort(coins.begin(), coins.end());
    
    dp[0] = 1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 1; j <= k; j++) {
            if (j - coins[i] >= 0) {
                dp[j] += dp[j - coins[i]];
            }
        }
    }
    
    cout << dp[k];

    return 0;
}
