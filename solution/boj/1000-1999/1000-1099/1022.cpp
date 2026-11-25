// BOJ 1022 - 소용돌이 예쁘게 출력하기
// 난이도: Gold III
// 푼 날짜: 2024-01-12
// https://www.acmicpc.net/problem/1022

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int f(int r, int c) {
    int k = max(abs(c), abs(r));
    
    int edge = 2 * k + 1;
    int sq = edge * edge;
    int psq = sq - 8 * k;
    
    if (c == k) {
        if (r == -k) return sq;
        else return psq + k + r;
    }
    else if (r == k) {
        return psq + 3 * k - c;
    }
    else if (c == -k) {
        return psq + 5 * k - r;
    }
    else {
        return sq - k + c;
    }
}

int main() {
    fastio;
    
    int r1, c1, r2, c2;
    cin >> r1 >> c1 >> r2 >> c2;
    
    size_t ml = 0;
    vector<vector<string>> arr(r2 - r1 + 1, vector<string>(c2 - c1 + 1));
    
    for (int i = r1; i <= r2; i++) {
        for (int j = c1; j <= c2; j++) {
            arr[i - r1][j - c1] = to_string(f(-i, j));
            ml = max(ml, arr[i - r1][j - c1].size());
        }
    }
    
    for (auto x: arr) {
        for (auto s: x) {
            cout << string(ml - s.size(), ' ') + s << " ";
        }
        cout << "\n";
    }
}
