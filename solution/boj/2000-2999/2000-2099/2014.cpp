// BOJ 2014 - 소수의 곱
// 난이도: Gold I
// 푼 날짜: 2023-09-15
// https://www.acmicpc.net/problem/2014

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

constexpr ll mod = 1e9;
ll limit = 1LL << 31;

priority_queue<ll> pq;
vector<ll> primes;

void backtrack(ll value, int n, int li) {
    if (value >= limit) return;
    
    bool end = false;
    if (pq.size() >= n) {
        if (pq.top() > value) {
            pq.pop();
            pq.push(value);
        }
        else end = true;
    }
    else if (value > 1) pq.push(value);
    
    if (end) return;
    
    for (int i = li; i < primes.size(); i++) {
        backtrack(value * primes[i], n, i);
    }
}

int main() {
    fastio;
    int k, n;
    cin >> k >> n;
    
    primes = vector<ll>(k);
    
    for (int i = 0; i < k; i++) {
        cin >> primes[i];
    }
    backtrack(1, n, 0);
    // cout << c << "\n";
    cout << pq.top();
}
