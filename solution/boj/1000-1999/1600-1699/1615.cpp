// BOJ 1615 - 교차개수세기
// 난이도: Platinum V
// 푼 날짜: 2023-09-20
// https://www.acmicpc.net/problem/1615

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

vector<ll> segtree;
vector<int> id;

void update(int x, int l, int r, int index) {
    if (l == r) {
        segtree[index]++;
        return;
    }
    
    int mid = (l + r) / 2;
    if (x <= mid) update(x, l, mid, 2 * index);
    else update(x, mid + 1, r, 2 * index + 1);
    
    segtree[index] = segtree[2 * index] + segtree[2 * index + 1];
}

ll query(int a, int b, int l, int r, int index) {
    if (a > b) return 0;
    
    if (l >= a && r <= b) {
        return segtree[index];
    }
    
    if (l > b || r < a) return 0;
    
    int mid = (l + r) / 2;
    return query(a, b, l, mid, 2 * index) + query(a, b, mid + 1, r, 2 * index + 1);
}

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    segtree = vector<ll>(4 * n);
    vector<pair<int, int>> edge(m);
    
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        
        edge[i] = make_pair(x, y);
    }
    sort(edge.begin(), edge.end());
    
    ll ans = 0;
    for (int i = 0; i < m; i++) {
        update(edge[i].second, 1, n, 1);
        ans += query(edge[i].second + 1, n, 1, n, 1);
    }
    cout << ans;
    
    return 0;
}
