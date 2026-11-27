// BOJ 2023 - 신기한 소수
// 난이도: Gold V
// 푼 날짜: 2023-04-15
// https://www.acmicpc.net/problem/2023

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

vector<int> sieve(int ceiling) {
    vector<bool> not_prime(ceiling, false);
    vector<int> primes;
    
    for (int i = 2; i < ceiling; i++) {
        if (!not_prime[ceiling]) {
            for (int j = 2; i * j < ceiling; j++) {
                not_prime[i * j] = true;
            }
            primes.push_back(i);
        }
    }
    return primes;
}

bool determine_prime(vector<int> &primes, int number) {
    if (number == 1) return false;
    for (int p: primes) {
        if (p * p > number) break;
        if (number % p == 0) return false;
    }
    return true;
}

void determine_amazing_prime(vector<int> &primes, int number, int digits, int N) {
    if (digits == N) {
        cout << number << "\n";
        return;
    }
    
    for (int i = 1; i <= 9; i++) {
        if (i % 2 == 0 && i != 2) continue;
        int next_number = number * 10 + i;
        if (determine_prime(primes, next_number)) {
            determine_amazing_prime(primes, next_number, digits + 1, N);
        }
    }
}

int main() {
    int N;
    cin >> N;
    
    vector<int> primes = sieve(10001);
    
    if (N == 1) {
        cout << "2\n3\n5\n7";
    }
    else {
        determine_amazing_prime(primes, 0, 0, N);
    }
    
    
    return 0;
}
