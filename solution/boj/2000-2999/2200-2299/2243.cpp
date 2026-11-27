// BOJ 2243 - 사탕상자
// 난이도: Platinum V
// 푼 날짜: 2023-09-21
// https://www.acmicpc.net/problem/2243

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

vector<int> segtree;

void update(int x, int delta, int l, int r, int index) {
    if (l == r) {
        segtree[index] += delta;
        return;
    }
    
    int mid = (l + r) / 2;
    if (x <= mid) update(x, delta, l, mid, 2 * index);
    else update(x, delta, mid + 1, r, 2 * index + 1);
    
    segtree[index] = segtree[2 * index] + segtree[2 * index + 1];
}

int query(int a, int b, int l, int r, int index) {
    if (l > b || r < a) return 0;
    if (l >= a && r <= b) return segtree[index];
    
    int mid = (l + r) / 2;
    return query(a, b, l, mid, 2 * index) + query(a, b, mid + 1, r, 2 * index + 1);
}

int main() {
    fastio;

    int n;
    cin >> n;
    
    int lim = 1000000;
    segtree = vector<int>(4 * (lim + 1));
    
    for (int i = 0; i < n; i++) {
        int qt;
        cin >> qt;
        
        if (qt == 1) {
            int rank;
            cin >> rank;
            
            int left = 1;
            int right = lim + 1;
            
            while (1) {
                int mid = (left + right) / 2;
                int candy = query(1, mid, 1, lim, 1);
                
                if (candy >= rank) {
                    if (query(1, mid - 1, 1, lim, 1) < rank) {
                        cout << mid << "\n";
                        update(mid, -1, 1, lim, 1);
                        break;
                    }
                    else right = mid;
                }
                else {
                    left = mid;
                }
            }
        }
        else {
            int taste, c;
            cin >> taste >> c;
            
            update(taste, c, 1, lim, 1);
        }
    }
    
    return 0;
}
