// BOJ 2011 - 암호코드
// 난이도: Gold V
// 푼 날짜: 2023-06-27
// https://www.acmicpc.net/problem/2011

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main()
{
    string s;
    cin >> s;
    
    int n = s.size();
    int mod = 1000000;
    vector<int> dp(n);
    
    for (int i = 0; i < n; i++) {
        int m1 = 0;
        int m2 = 0;
        
        if (i == 0) {
            if (s[i] != '0') {
                m1++;
            }
            dp[i] = m1;
        }
        else if (s[i] != '0') {
            m1 = dp[i - 1];
        }
        
        if (i > 0) {
            int two_value = stoi(s.substr(i - 1, 2));
            // cout << two_value << "\n";
            
            if (two_value >= 10 && two_value <= 26) {
                if (i > 1)
                    m2 = dp[i - 2];
                else m2++;
            }
        }
        dp[i] = (m1 + m2) % mod;
        // cout << "dp[" << i << "] = " << dp[i] << "\n";
    }
    
    cout << dp[n - 1];

    return 0;
}
