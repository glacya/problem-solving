// BOJ 1588 - 수열
// 난이도: Gold I
// 푼 날짜: 2023-10-31
// https://www.acmicpc.net/problem/1588

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

class Case {
public:
    ll v1;
    ll v2;
    ll v3;
    
    Case() {}
    Case(ll v1, ll v2, ll v3) {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    }
    
    Case operator+(const Case &c) const {
        return Case(this->v1 + c.v1, this->v2 + c.v2, this->v3 + c.v3);
    }
};

vector<vector<Case>> dp;
vector<vector<ll>> division = {{0, 2, 1}, {1, 0, 0}, {1, 2, 1}};

Case query(ll current_num, ll a, ll b, ll l, ll r, ll depth) {
    // cout << "query(" << current_num << "," << a << "," << b << "," << l << "," << r << "," << depth << ")\n";
    if (l <= a && r >= b) {
        return dp[depth][current_num];
    }
    
    if (l > b || r < a) {
        return Case(0, 0, 0);
    }
    
    ll d1 = (2 * a + b + 1) / 3;
    ll d2 = (a + 2 * (b + 1)) / 3;
    
    Case q1 = query(division[current_num][0], a, d1 - 1, l, r, depth - 1);
    Case q2 = query(division[current_num][1], d1, d2 - 1, l, r, depth - 1);
    Case q3 = query(division[current_num][2], d2, b, l, r, depth - 1);
    
    return q1 + q2 + q3;
}

int main() {
    fastio;
    
    ll init, left, right, n;
    cin >> init >> left >> right >> n;
    
    init--;
    
    dp = vector<vector<Case>>(n + 1, vector<Case>(3));
    dp[0][0] = Case(1, 0, 0);
    dp[0][1] = Case(0, 1, 0);
    dp[0][2] = Case(0, 0, 1);
    
    for (ll i = 1; i <= n; i++) {
        dp[i][0] = dp[i - 1][0] + dp[i - 1][1] + dp[i - 1][2];
        dp[i][1] = dp[i - 1][1] + dp[i - 1][0] + dp[i - 1][0];
        dp[i][2] = dp[i - 1][2] + dp[i - 1][1] + dp[i - 1][1];
    }
    
    ll p3 = 1;
    for (ll i = 0; i < n; i++) 
        p3 *= 3;
    
    Case total = query(init, 0, p3 - 1, left, right, n);
    
    cout << total.v1 << " " << total.v2 << " " << total.v3;
}
