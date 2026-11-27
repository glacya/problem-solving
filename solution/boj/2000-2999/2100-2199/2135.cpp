// BOJ 2135 - 문자열 압축하기
// 난이도: Platinum V
// 푼 날짜: 2025-02-05
// https://www.acmicpc.net/problem/2135

#include <bits/stdc++.h>

#define ll long long
#define fastio ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);

using namespace std;

// pair<int, string> separate_repetition_count(string &str) {
//     int i = 0;
//     int len = str.length();

//     while (i < len && str[i] != '(') {
//         i++;
//     }
    
//     if (i == len) {
//         return make_pair(1, str);
//     }
//     else {
//         return make_pair(stoi(str.substr(0, i)), str.substr(i + 1, len - i - 2));
//     }
// }

int get_string_length(pair<int, string> &p) {
    if (p.second == "") {
        return 1000000;
    }

    if (p.first == 1) {
        return p.second.length();
    }
    else {
        return min(p.first * p.second.length(), to_string(p.first).length() + 2 + p.second.length());
    }
}

pair<int, string> merge(pair<int, string> &a, pair<int, string> &b) {
    // auto p1 = separate_repetition_count(a);
    // auto p2 = separate_repetition_count(b);

    if (a.second == b.second) {
        return make_pair(a.first + b.first, a.second);
    }
    else {
        string merged = "";

        string count_a = to_string(a.first);
        string count_b = to_string(b.first);

        if (count_a.length() + 2 + a.second.length() <= a.first * a.second.length()) {
            merged += count_a + "(" + a.second + ")";
        }
        else {
            for (int i = 0; i < a.first; i++) {
                merged += a.second;
            }
        }

        if (count_b.length() + 2 + b.second.length() <= b.first * b.second.length()) {
            merged += count_b + "(" + b.second + ")";
        }
        else {
            for (int i = 0; i < b.first; i++) {
                merged += b.second;
            }
        }

        return make_pair(1, merged);
    }
}

int main() {
    fastio;

    string original;
    cin >> original;

    int n = original.length();

    vector<vector<pair<int, string>>> dp(n, vector<pair<int, string>>(n));

    for (int i = 0; i < n; i++) {
        dp[i][i] = make_pair(1, string(1, original[i]));
    }

    for (int diff = 1; diff < n; diff++) {
        for (int col = 0; col + diff < n; col++) {
            // cout << "Computing DP table of row " << col << ", " << col + diff << "\n";

            for (int pick = col; pick < col + diff; pick++) {
                pair<int, string> computed_now = merge(dp[col][pick], dp[pick + 1][col + diff]);

                int old_length = get_string_length(dp[col][col + diff]);
                int new_length = get_string_length(computed_now);

                if (old_length > new_length) {
                    dp[col][col + diff] = computed_now;
                }
                else if (old_length == new_length && computed_now.first > dp[col][col + diff].first) {
                    dp[col][col + diff] = computed_now;
                }

                // cout << "  Tried " << col << ", " << pick << " and " << pick + 1 << ", " << col + diff << ".\n";
                // cout << "\t" << dp[col][pick].first << ", " << dp[col][pick].second << "\n";
                // cout << "\t" << dp[pick + 1][col + diff].first << ", " << dp[pick + 1][col + diff].second << "\n";

                // cout << "  ..Now the result is " << dp[col][col + diff].first << ", " << dp[col][col + diff].second << "\n";
            }

            // cout << "Cell Result = " << dp[col][col + diff].first << ", " << dp[col][col + diff].second << "\n";
        }
    }

    // cout << dp[0][n - 1].first << " " << dp[0][n - 1].second << "\n";
    cout << get_string_length(dp[0][n - 1]) << "\n";
}
