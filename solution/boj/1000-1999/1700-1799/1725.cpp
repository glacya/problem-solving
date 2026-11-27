// BOJ 1725 - 히스토그램
// 난이도: Platinum V
// 푼 날짜: 2023-12-14
// https://www.acmicpc.net/problem/1725

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

ll histogram(vector<ll> &arr, int left, int right) {
    // cout << "Histogram(" << left << ", " << right << ")\n";
    
    if (left == right) return arr[left];
    
    int mid = (left + right) / 2;
    ll lv = histogram(arr, left, mid);
    ll rv = histogram(arr, mid + 1, right);
    
    int l = mid;
    int r = mid + 1;
    
    ll minh = min(arr[l], arr[r]);
    ll cv = minh * 2ll;
    ll w = 2;
    
    while (l > left || r < right) {
        if (l <= left) {
            minh = min(arr[r + 1], minh);
            r++;
        }
        else if (r >= right) {
            minh = min(arr[l - 1], minh);
            l--;
        }
        else {
            ll lh = arr[l - 1];
            ll rh = arr[r + 1];
            
            if (rh >= lh) {
                minh = min(rh, minh);
                r++;
            }
            else {
                minh = min(lh, minh);
                l--;
            }
        }
        cv = max(cv, (w + 1) * minh);
        w++;
    }
    
    // cout << "Histogram(" << left << ", " << right << ") returned " << max(cv, max(lv, rv)) << "\n";
    
    return max(cv, max(lv, rv));
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    vector<ll> arr(n);
    
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << histogram(arr, 0, n - 1);
}
