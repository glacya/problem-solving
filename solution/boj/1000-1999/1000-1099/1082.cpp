// BOJ 1082 - 방 번호
// 난이도: Gold III
// 푼 날짜: 2023-06-30
// https://www.acmicpc.net/problem/1082

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int N, M;
    cin >> N;
    vector<int> cost(N);
    
    for (int i = 0; i < N; i++) {
        cin >> cost[i];
    }
    cin >> M;
    
    vector<string> dp(M + 1, "");
    
    for (int lim = 1; lim <= M; lim++) {
        for (int i = 0; i < N; i++) {
            if (cost[i] <= lim) {
                if (i == 0 && dp[lim - cost[i]].size() == 0) continue;
                
                int index = 0;
                for (char c: dp[lim - cost[i]]) {
                    if (c <= (char)(i + '0')) {
                        break;
                    }
                    index++;
                }
                string new_str = dp[lim - cost[i]].substr(0, index);
                new_str += (char) (i + '0');
                new_str += dp[lim - cost[i]].substr(index, string::npos);
                
                // cout << "Constructing new_str:" << new_str << "\n";
                
                if (dp[lim].size() < new_str.size()) {
                    dp[lim] = new_str;
                }
                else if (dp[lim].size() == new_str.size() && dp[lim] < new_str) {
                    dp[lim] = new_str;
                }
            }
        }
    }
    
    if (dp[M].size() == 0) cout << 0;
    else cout << dp[M];
    
    return 0;
}
