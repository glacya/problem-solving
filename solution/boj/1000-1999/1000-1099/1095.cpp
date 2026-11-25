// BOJ 1095 - 마법의 구슬
// 난이도: Platinum V
// 푼 날짜: 2023-11-15
// https://www.acmicpc.net/problem/1095

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    ll s, f, m;
    cin >> s >> f >> m;
    
    vector<ll> prime;
    vector<bool> isp(m + 1);
    
    for (ll i = 2; i <= m; i++) {
        if (!isp[i]) {
            prime.push_back(i);
            
            for (ll j = 2; i * j <= m; j++) {
                isp[i * j] = true;
            }
        }
    }
    
    vector<ll> factors(m + 1);
    
    for (ll p: prime) {
        ll x = p;
        ll cnt = 0;
        
        while (x <= m) {
            x *= p;
            cnt++;
        }
        
        ll sv = s + f;
        ll sc = 0;
        
        while (sv) {
            sc += sv / p;
            sv /= p;
        }
        
        ll fv = s;
        ll fc = 0;
        
        while (fv) {
            fc += fv / p;
            fv /= p;
        }
        
        ll rv = f;
        ll rc = 0;
        
        while (rv) {
            rc += rv / p;
            rv /= p;
        }
        
        factors[p] = min(cnt, sc - fc - rc);
    }
    
    for (ll i = m; i >= 1; i--) {
        ll n = i;
        ll x = 2;
        bool failed = false;
        for (int j = 0; j < prime.size() && prime[j] * prime[j] <= n; j++) {
            ll f = 0;
            ll x = prime[j];
            while (n % x == 0) {
                n /= x;
                f++;
            }
            
            if (factors[x] < f) {
                failed = true;
                break;
            }
        }
        
        if (n > 1) {
            if (factors[n] == 0) failed = true;
        }
        
        if (!failed) {
            cout << i;
            break;
        }
    }
}
