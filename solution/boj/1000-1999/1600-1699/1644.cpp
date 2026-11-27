// BOJ 1644 - 소수의 연속합
// 난이도: Gold III
// 푼 날짜: 2023-03-10
// https://www.acmicpc.net/problem/1644

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    int N;
    cin >> N;
    
    vector<bool> not_prime(N + 1, false);
    vector<int> primes;
    int num_of_cases = 0;
    
    not_prime[0] = true;
    not_prime[1] = true;
    
    for (int i = 2; i < N + 1; i++) {
        if (!not_prime[i]) {
            for (int j = 2; i * j < N + 1; j++) {
                not_prime[i * j] = true;
            }
            primes.push_back(i);
        }
    }
    
    long long acc = 0;
    int start = 0;
    int end = 0;
    
    while (1) {
        if (acc < N) {
            if (end == primes.size()) {
                break;
            }
            acc += primes[end++];
        }
        else if (acc > N) {
            acc -= primes[start++];
        }
        else {
            num_of_cases++;
            // cout << start << ", " << end << endl;
            if (end < primes.size())
                acc += primes[end++];
            else break;
        }
    }
    
    cout << num_of_cases;

    return 0;
}
