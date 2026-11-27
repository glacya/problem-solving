// BOJ 2156 - 포도주 시식
// 난이도: Silver I
// 푼 날짜: 2023-05-23
// https://www.acmicpc.net/problem/2156

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
#define fio ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);

int main()
{
    fio;
    int n;
    cin >> n;
    
    // dp[x][0]: Maximum when x-th glass is the first wine in a row.
    // dp[x][1]: Maximum when x-th glass is the second wine in a row.
    // dp[x][2]: Maximum when x-th glass is ignored..
    vector<vector<int>> dp(n);
    vector<int> wine(n);
    
    for (int i = 0; i < n; i++) {
        cin >> wine[i];
    }
    
    if (n == 1) cout << wine[0];
    else if (n == 2) cout << wine[0] + wine[1];
    else {
        dp[0] = {wine[0], wine[0], 0};
        dp[1] = {wine[1], wine[0] + wine[1], wine[0]};
        
        int peak = wine[0] + wine[1];
        for (int i = 2; i < n; i++) {
            int on_first = max(max(dp[i - 2][0], dp[i - 2][1]), dp[i - 1][2]) + wine[i];
            int on_second = dp[i - 1][0] + wine[i];
            
            dp[i] = {on_first, on_second, peak};
            peak = max(peak, max(on_first, on_second));
        }
        
        // for (int i = 0; i < n; i++) {
        //     cout << dp[i][0] << " " << dp[i][1] << endl;
        // }
        
        cout << peak;
    }
    
    
}
