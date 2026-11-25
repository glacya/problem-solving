// BOJ 1153 - 네 개의 소수
// 난이도: Gold III
// 푼 날짜: 2023-10-01
// https://www.acmicpc.net/problem/1153

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    if (n < 8) {
        cout << "-1";
        return 0;
    }
    
    vector<bool> is_prime(n + 1, true);
    vector<int> primes;
    
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (int j = 2; i * j <= n; j++) {
                is_prime[i * j] = false;
            }
        }
    }
    
    if (n % 2) {
        cout << "2 3 ";
        
        for (int p: primes) {
            if (is_prime[n - p - 5]) {
                cout << p << " " << n - p - 5;
                break;
            }
        }
    }
    else {
        int a = n / 2;
        int b = n / 2;
        
        if (a % 2) {
            a++;
            b--;
        }
        
        for (int p: primes) {
            if (is_prime[a - p]) {
                cout << p << " " << a - p << " ";
                break;
            }
        }
        
        for (int p: primes) {
            if (is_prime[b - p]) {
                cout << p << " " << b - p;
                break;
            }
        }
    }
}
