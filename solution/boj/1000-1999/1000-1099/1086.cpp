// BOJ 1086 - 박성원
// 난이도: Platinum V
// 푼 날짜: 2023-09-17
// https://www.acmicpc.net/problem/1086

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

int bx = 0;

void backtrack(vector<vector<int>> &bitmasks, int bm, int li, int depth, int n) {
    if (depth == n) return;
    
    bitmasks[depth].push_back(bm);
    
    for (int i = li + 1; i < n; i++) {
        backtrack(bitmasks, bm | (1 << i), i, depth + 1, n);
    }
}

ll factorial(ll x) {
    if (x == 0) return 1;
    else return x * factorial(x - 1);
}

ll gcd(ll x, ll y) {
    if (x < y) swap(x, y);
    
    if (y == 0) return x;
    if (y == 1) return 1;
    return gcd(y, x % y);
}

ll power(ll x, ll p, ll mod) {
    ll res = 1;
    ll fr = x;
    
    while (p) {
        if (p % 2) res = (res * fr) % mod;
        fr = (fr * fr) % mod;
        
        p /= 2;
    }
    return res;
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    vector<string> ss(n);
    vector<ll> elems(n);
    vector<ll> digits(n);
    vector<vector<int>> bitmasks(n);
    vector<ll> mods(751);
    
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        
        ll sz = (ll) s.size();
        ss[i] = s;
        
        digits[i] = sz;
    }
    
    backtrack(bitmasks, 0, -1, 0, n);
    
    ll k;
    cin >> k;
    
    for (int i = 0; i < n; i++) {
        ll ans = 0;
        ll fr = 1;
        for (int j = digits[i] - 1; j >= 0; j--) {
            ans = (ans + ((ll) (ss[i][j] - '0')) * fr) % k;
            fr = (fr * 10) % k;
        }
        elems[i] = ans;
    }
    
    mods[0] = 1;
    for (int i = 1; i <= 750; i++) {
        mods[i] = (mods[i - 1] * 10) % k;
    }
    
    // pair(number_of_cases, digits_total)
    vector<vector<pair<ll, ll>>> dp(k, vector<pair<ll, ll>>(1 << n));
    dp[0][0] = make_pair(1, 0);
    
    for (int bits = 0; bits < n; bits++) {
        for (int bm: bitmasks[bits]) {
            for (int r = 0; r < k; r++) {
                for (int i = 0; i < n; i++) {
                    if (!(bm & (1 << i))) {
                        int next_bm = bm | (1 << i);
                        ll next_mod = mods[dp[r][bm].second] * elems[i] + r;
                        next_mod %= k;
                        
                        dp[next_mod][next_bm].first += dp[r][bm].first;
                        dp[next_mod][next_bm].second = dp[r][bm].second + digits[i];
                    }
                }
            }
        }
    }
    
    ll num = dp[0][(1 << n) - 1].first;
    ll fac = factorial((ll) n);
    ll g = gcd(num, fac);
    
    if (num == 0) {
        cout << "0/1";
    }
    else if (num == fac) {
        cout << "1/1";
    }
    else {
        num /= g;
        fac /= g;
        cout << num << "/" << fac;
    }
    
    return 0;
}
