// BOJ 7795 - 먹을 것인가 먹힐 것인가
// 난이도: Silver III
// 푼 날짜: 2026-11-25
// https://www.acmicpc.net/problem/7795

#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using ld = long double;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<int> a(n);
        vector<int> b(m);

        for (int i = 0; i < n; i++) cin >> a[i];
        for (int i = 0; i < m; i++) cin >> b[i];

        sort(a.begin(), a.end(), greater<int>());
        sort(b.begin(), b.end(), greater<int>());

        int pairs = 0;
        int bp = 0;

        for (int i = 0; i < n; i++) {
            while (bp < m && a[i] <= b[bp]) {
                bp++;
            }

            pairs += m - bp;
        }

        cout << pairs << "\n";
    }

    return 0;
}
