// BOJ 1849 - 순열
// 난이도: Platinum IV
// 푼 날짜: 2023-12-09
// https://www.acmicpc.net/problem/1849

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

vector<int> segtree;

int query(int a, int b, int l, int r, int index) {
    if (a > b) return 0;
    if (l > b || r < a) return 0;
    if (l >= a && r <= b) return segtree[index];
    
    int mid = (l + r) / 2;
    
    return query(a, b, l, mid, 2 * index) + query(a, b, mid + 1, r, 2 * index + 1);
}

void update(int x, int l, int r, int index) {
    if (l == r) {
        segtree[index] = 1;
        return;
    }
    
    int m = (l + r) / 2;
    
    if (x <= m) update(x, l, m, 2 * index);
    else update(x, m + 1, r, 2 * index + 1);
    
    segtree[index] = segtree[2 * index] + segtree[2 * index + 1];
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    segtree = vector<int>(4 * n);
    vector<int> arr(n + 1);
    
    for (int i = 1; i <= n; i++) {
        int v;
        cin >> v;
        
        int left = 1;
        int right = n + 1;
        
        // cout << "Query " << i << ": " << v << "\n";
        while (left + 1 < right) {
            int mid = (left + right) / 2;
            int q = query(1, mid - 1, 1, n, 1);
            int step = mid - 1 - q;
            
            // cout << "  mid = " << (left + right) / 2 << ", ";
            // cout << " q = " << q << ", step = " << step << "\n";
            
            if (step <= v) {
                // cout << "  searching right region..\n";
                left = mid;
            }
            else {
                // cout << "  searching left region..\n";
                right = mid;
            }
        }
        
        // cout << " result = " << left << "\n";
        
        arr[left] = i;
        update(left, 1, n, 1);
    }
    
    for (int i = 1; i <= n; i++) {
        cout << arr[i] << "\n";
    }
}
