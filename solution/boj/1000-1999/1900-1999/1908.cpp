// BOJ 1908 - 곱셈 전개식
// 난이도: Platinum V
// 푼 날짜: 2023-06-15
// https://www.acmicpc.net/problem/1908

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

ll p2(ll x) {
    ll y = x;
    ll ans = 1;
    ll fr = 2;
    while (y) {
        if (y % 2) ans = (ans * fr) % 10000;
        fr = (fr * fr) % 10000;
        y /= 2;
    }
    return ans;
}

int main()
{
    ll n;
    cin >> n;
    
    ll mod = 10000;
    ll total = 0;
    
    ll nm1 = p2(n - 1);
    
    // The number of x's
    total = (total + n) % mod;
    
    // The number of plus'
    total = (total + nm1 * 2 + mod - 1) % mod;
    
    // The number of a's
    total = (total + (n % mod) * nm1) % mod;
    
    // The number of parentheses
    total = (total + 2 * (n % mod) + mod - 2) % mod;
    
    ll digits = 0;
    ll base = 0;
    while (n > base) {
        digits = (digits + n - base) % mod;
        base = base * 10 + 9;
    }
    
    // The number of superscripts
    total = (total + digits + mod - 1) % mod;
    
    // The number of subscripts
    total = (total + digits * nm1) % mod;
    
    cout << total;

    return 0;
}
