// BOJ 1321 - 군인
// 난이도: Platinum V
// 푼 날짜: 2023-10-01
// https://www.acmicpc.net/problem/1321

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

vector<pair<ll, ll>> segtree;

void update(int i, ll v, int l, int r, int index) {
    if (l == r) {
        segtree[index].first += v;
        return;
    }
    
    int mid = (l + r) / 2;
    if (i <= mid) update(i, v, l, mid, 2 * index);
    else update(i, v, mid + 1, r, 2 * index + 1);
    
    segtree[index].first = segtree[2 * index].first + segtree[2 * index].second;
    segtree[index].second = segtree[2 * index + 1].first + segtree[2 * index + 1].second;
}

int query(ll x, int l, int r, int index) {
    if (l == r) return l;
    
    int mid = (l + r) / 2;
    
    if (x <= segtree[index].first) {
        return query(x, l, mid, 2 * index);
    }
    else {
        return query(x - segtree[index].first, mid + 1, r, 2 * index + 1);
    }
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    segtree = vector<pair<ll, ll>>(4 * n);
    
    for (int i = 1; i <= n; i++) {
        ll v;
        cin >> v;
        update(i, v, 1, n, 1);
    }
    
    int m;
    cin >> m;
    
    while (m--) {
        int q;
        cin >> q;
        
        if (q == 1) {
            int x;
            ll v;
            cin >> x >> v;
            
            update(x, v, 1, n, 1);
        }
        else {
            ll x;
            cin >> x;
            
            cout << query(x, 1, n, 1) << "\n";
        }
    }
}
