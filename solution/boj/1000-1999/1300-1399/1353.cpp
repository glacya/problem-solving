// BOJ 1353 - 합과 곱
// 난이도: Platinum V
// 푼 날짜: 2023-03-01
// https://www.acmicpc.net/problem/1353

#include <iostream>
#include <numbers>
#include <cmath>

using namespace std;

typedef long long ll;
typedef long double ld;

constexpr ld e = 2.71828182845904523536;

bool inequality(ld n, ld S, ld P) {
    // cout << "inequality(" << n << ")\n";
    // ld lhs = n * log(S / n);
    // ld rhs = log(P);
    ld lhs = S / n;
    ld rhs = pow(P, 1 / n);
    return lhs - rhs >= 0;
}

ll compute_minimum_size(ld S, ld P) {
    if (S == P) return 1;
    
    if (S / e < log(P)) return -1;
    
    ll left = 1;
    ll right = floor(S / e);
    ll n = (left + right) / 2;

    
    if (inequality((ld) 1, S, P)) return 2;
    
    if (!inequality((ld) right, S, P)) {
        if (!inequality((ld) (right + 1), S, P)) {
            return -1;
        }
        else return right + 1;
    }
    
    while (1) {
        if (inequality((ld) n, S, P)) {
            if (!inequality((ld) (n - 1), S, P)) {
                return n;
            }
            else {
                right = (left + right) / 2;
                n = (left + n) / 2;
            }
        }
        else {
            left = (left + right) / 2;
            
            if ((n + right) / 2 == n) {
                if (inequality((ld) (n + 1), S, P)) return n + 1;
            }
            
            n = (n + right) / 2;
            
        }
    }
    
    return -1;
}

int main() {
    ld S, P;
    cin >> S >> P;
    
    cout << compute_minimum_size(S, P);

    return 0;
}
