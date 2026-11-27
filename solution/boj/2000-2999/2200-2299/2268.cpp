// BOJ 2268 - 수들의 합 7
// 난이도: Gold I
// 푼 날짜: 2023-08-26
// https://www.acmicpc.net/problem/2268

#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

vector<ll> segtree;

ll sum(int a, int b, int left, int right, int index) {
    // cout << "sum(" << a << ", " << b << ", " << left << ", " << right << ")\n";
    int mid = (left + right) / 2;
    
    if (a <= left && b >= right) {
        return segtree[index];
    }
    
    if (b < left || a > right) {
        return 0;
    }
    
    return sum(a, b, left, mid, 2 * index) + sum(a, b, mid + 1, right, 2 * index + 1);
}

ll modify(int target, ll value, int left, int right, int index) {
    int mid = (left + right) / 2;
    
    if (left == right) {
        ll prev = segtree[index];
        segtree[index] = value;
        return value - prev;
    }
    
    ll delta = 0;
    if (target <= mid) {
        delta = modify(target, value, left, mid, 2 * index);
    }
    else {
        delta = modify(target, value, mid + 1, right, 2 * index + 1);
    }
    
    segtree[index] += delta;
    return delta;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int n, m;
    cin >> n >> m;
    
    segtree = vector<ll>(4 * (n + 1));
    
    while (m--) {
        int q;
        cin >> q;
        
        if (q == 0) {
            int i, j;
            cin >> i >> j;
            
            if (i > j) swap(i, j);
            cout << sum(i, j, 1, n, 1) << "\n";
        }
        else {
            int i;
            ll k;
            cin >> i >> k;
            modify(i, k, 1, n, 1);
        }
    }

    return 0;
}
