// BOJ 1509 - 팰린드롬 분할
// 난이도: Gold I
// 푼 날짜: 2023-06-07
// https://www.acmicpc.net/problem/1509

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    string s;
    cin >> s;
    
    vector<pair<int, int>> palins;
    vector<int> dp(s.size(), 0x7ffffff0);
    dp[0] = 1;
    
    for (int index = 0; index < s.size(); index++) {
        // cout << "At index " << index << "\n";
        int i1 = index + 2;
        if (index + 1 < s.size() && s[index + 1] == s[index]) {
            while (i1 < s.size()) {
                if (s[i1] != s[2 * index + 1 - i1]) break;
                // palins.push_back(make_pair(2 * index + 2 - i1, i1));
                // cout << "\tFound palindrome from " << 2 * index + 2 - i1 << " to " << i1 - 1 << "\n";
                
                if (2 * index + 2 - i1 != i1 - 1)
                    dp[i1 - 1] = (2 * index + 1 - i1 >= 0) ? min(dp[2 * index + 1 - i1] + 1, dp[i1 - 1]) : 1;
                i1++;
            }
            // palins.push_back(make_pair(2 * index + 2 - i1, i1));
            // cout << "\tFound palindrome from " << 2 * index + 2 - i1 << " to " << i1 - 1 << "\n";
            
            dp[i1 - 1] = (2 * index + 1 - i1 >= 0) ? min(dp[2 * index + 1 - i1] + 1, dp[i1 - 1]) : 1;
            
        }
        
        int i2 = index + 2;
        if (index + 2 < s.size() && s[index + 2] == s[index]) {
            while (i2 < s.size()) {
                if (s[i2] != s[2 * index + 2 - i2]) break;
                // palins.push_back(make_pair(2 * index + 3 - i2, i2));
                // cout << "\tFound palindrome from " << 2 * index + 3 - i2 << " to " << i2 - 1 << "\n";
                if (i2 - 1 != 2 * index + 3 - i2)
                    dp[i2 - 1] = (2 * index + 2 - i2 >= 0) ? min(dp[2 * index + 2 - i2] + 1, dp[i2 - 1]) : 1;
                i2++;
            }
            // palins.push_back(make_pair(2 * index + 3 - i2, i2));
            // cout << "\tFound palindrome from " << 2 * index + 3 - i2 << " to " << i2 - 1 << "\n";
            dp[i2 - 1] = (2 * index + 2 - i2 >= 0) ? min(dp[2 * index + 2 - i2] + 1, dp[i2 - 1]) : 1;
        }
        
        if (index > 0) {
            dp[index] = min(dp[index], dp[index - 1] + 1);
        }
        // cout << "\t\tdp[" << index << "] = " << dp[index] << "\n";
    }
    
    // for (auto p: palins) {
    //     cout << p.first << " " << p.second - 1 << " ";
    //     cout << s.substr(p.first, p.second - p.first) << "\n";
    // }
    
    cout << dp[s.size() - 1];

    return 0;
}
