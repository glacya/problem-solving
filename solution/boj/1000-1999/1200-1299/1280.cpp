// BOJ 1280 - 나무 심기
// 난이도: Platinum IV
// 푼 날짜: 2023-09-23
// https://www.acmicpc.net/problem/1280

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

constexpr ll mod = 1e9 + 7;

vector<pair<ll, ll>> segtree;

pair<ll, ll> query(int a, int b, int l, int r, int index) {
    if (l > b || r < a) return make_pair(0, 0);
    
    if (l >= a && r <= b) return segtree[index];
    
    int mid = (l + r) / 2;
    auto p1 = query(a, b, l, mid, 2 * index);
    auto p2 = query(a, b, mid + 1, r, 2 * index + 1);
    return make_pair((p1.first + p2.first) % mod, (p1.second + p2.second) % mod);
}

void update(int x, ll v, int l, int r, int index) {
    if (l == r) {
        segtree[index].first += v;
        segtree[index].first %= mod;
        segtree[index].second++;
        return;
    }
    
    int mid = (l + r) / 2;
    if (x <= mid) update(x, v, l, mid, 2 * index);
    else update(x, v, mid + 1, r, 2 * index + 1);
    
    auto p1 = segtree[2 * index];
    auto p2 = segtree[2 * index + 1];
    
    segtree[index] = make_pair((p1.first + p2.first) % mod, (p1.second + p2.second) % mod);
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    ll lim = 199999;
    segtree = vector<pair<ll, ll>>(4 * lim);
    
    ll ans = 1;
    for (int i = 0; i < n; i++) {
        ll x;
        cin >> x;
        
        update(x, x, 0, lim, 1);
        
        if (i > 0) {
            auto pl = query(0, x - 1, 0, lim, 1);
            auto pr = query(x + 1, lim, 0, lim, 1);
            
            ll res = (pl.second * x) % mod;
            res = (res + mod - ((pr.second * x) % mod)) % mod;
            res = (res + mod - pl.first + pr.first) % mod;
            ans = (ans * res) % mod;
        }
    }
    cout << ans;
    
    return 0;
}
