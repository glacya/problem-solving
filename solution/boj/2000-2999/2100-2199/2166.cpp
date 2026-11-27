// BOJ 2166 - 다각형의 면적
// 난이도: Gold V
// 푼 날짜: 2023-04-19
// https://www.acmicpc.net/problem/2166

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

int main() {
    int N;
    cin >> N;
    
    vector<ll> xs(N);
    vector<ll> ys(N);
    for (int i = 0; i < N; i++) {
        cin >> xs[i] >> ys[i];
    }
    
    ll total = 0;
    for (int i = 0; i < N; i++) {
        total += (xs[i] * ys[(i + 1) % N] - xs[(i + 1) % N] * ys[i]);
    }
    
    cout << fixed;
    cout.precision(1);
    cout << ((double) std::abs(total)) / 2.0;

    return 0;
}
