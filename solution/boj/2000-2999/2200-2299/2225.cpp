// BOJ 2225 - 합분해
// 난이도: Gold V
// 푼 날짜: 2023-08-03
// https://www.acmicpc.net/problem/2225

#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

constexpr ll mod = 1e9;

int main()
{
    int n, k;
    cin >> n >> k;
    
    vector<vector<ll>> dp(k + 1, vector<ll>(n + 1));
    
    dp[0][0] = 1;
    
    for (int i = 1; i <= k; i++) {
        for (int j = 0; j <= n; j++) {
            for (int v = 0; v <= j; v++) {
                dp[i][j] = (dp[i][j] + dp[i - 1][j - v]) % mod;
            }
        }
    }
    
    cout << dp[k][n];

    return 0;
}
