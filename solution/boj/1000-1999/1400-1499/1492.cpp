// BOJ 1492 - 합
// 난이도: Platinum II
// 푼 날짜: 2023-05-10
// https://www.acmicpc.net/problem/1492

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

ll mod = 1e9 + 7;

ll power(ll base, ll x) {
    ll frame = base;
    ll result = 1;
    ll p = x;
    
    while (p > 0) {
        if (p % 2) result = (result * frame) % mod;
        frame = (frame * frame) % mod;
        p /= 2;
    }
    
    return result;
}

ll inv(ll v) {
    return power(v, mod - 2);
}

vector<ll> mul_n(vector<ll> &poly) {
    vector<ll> output(poly);
    output.push_back(0);
    
    for (int i = (int) output.size() - 1; i >= 0; i--) {
        output[i + 1] = output[i];
    }
    output[0] = 0;
    
    return output;
}

vector<ll> mul_const(vector<ll> &poly, ll constant) {
    vector<ll> output(poly);
    
    for (int i = 0; i < output.size(); i++) {
        output[i] = (output[i] * constant) % mod;
    }
    
    return output;
}

vector<ll> add(vector<ll> &v1, vector<ll> &v2) {
    unsigned int output_size = max(v1.size(), v2.size());
    vector<ll> output(output_size, 0);
    
    for (unsigned int i = 0; i < output_size; i++) {
        if (i < v1.size()) {
            output[i] += v1[i];
        }
        if (i < v2.size()) {
            output[i] += v2[i];
        }
        output[i] %= mod;
    }
    
    return output;
}

int main()
{
    ll n, p;
    cin >> n >> p;
    
    vector<vector<ll>> dp;
    dp.push_back(vector<ll>{0, 1});
    
    for (ll i = 1; i <= p; i++) {
        vector<ll> next = mul_n(dp[i - 1]);
        next = add(next, dp[i - 1]);
        
        // Problematic Part: O(n^3)
        for (ll j = 0; j < i; j++) {
            vector<ll> jth_term = mul_const(dp[j], -dp[i - 1][j]);
            next = add(next, jth_term);
        }
        
        next = mul_const(next, i);
        next = mul_const(next, inv(i + 1));
        
        for (ll j = 0; j < next.size(); j++) {
            if (next[j] < 0) next[j] += mod;
        }
        
        dp.push_back(next);
    }
    
    // for (auto v: dp) {
    //     for (auto x: v) {
    //         cout << x << " ";
    //     }   
    //     cout << endl;
    // }
    
    ll output = 0;
    ll term = 1;
    for (int i = 0; i < dp[p].size(); i++) {
        output = (output + ((dp[p][i] * term) % mod)) % mod;
        term = (term * n) % mod;
    }
    
    cout << output;

    return 0;
}
