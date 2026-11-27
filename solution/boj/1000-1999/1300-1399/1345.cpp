// BOJ 1345 - 등차수열
// 난이도: Gold IV
// 푼 날짜: 2023-10-05
// https://www.acmicpc.net/problem/1345

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n;
    ll a0;
    cin >> n >> a0;
    
    if (n == 0) {
        cout << "0.0";
        return 0;
    }
    
    ll rem = 1e11;
    a0 *= rem;
    
    ll arr_sum = 0;
    vector<ll> arr(n + 1);
    arr[0] = a0;
    
    bool diff_succ = true;
    for (int i = 1; i <= n && diff_succ; i++) {
        cin >> arr[i];
        arr_sum += arr[i];
        arr[i] *= rem;
        
        for (int j = 0; j < i; j++) {
            if (arr[i] < arr[j]) {
                diff_succ = false;
                break;
            }
        }
    }
    
    if (!diff_succ) {
        cout << -1;
        return 0;
    }
    
    // cout << "Array sum = " << arr_sum  << "\n";
    ll left = arr[1] - arr[0] - 1;
    ll right = arr[1] - arr[0] + rem;
    
    while (left + 1 < right) {
        ll mid = (left + right) / 2;
        
        ll this_sum = 0;
        for (ll i = 1; i <= n; i++) {
            ll ai = arr[0] + i * mid;
            this_sum += ai / rem + (ai < 0 && ai % rem ? -1 : 0);
        }
        
        // cout << "Tested " << mid << ": " << this_sum << "\n";
        
        if (this_sum >= arr_sum) {
            right = mid;
        }
        else {
            left = mid;
        }
    }
    
    // Validity check
    ll ans = right;
    // cout << "Mid ans = " << ans << ", now checking validity of solution..\n";
    
    bool valid = true;
    for (ll i = 1; i <= n; i++) {
        ll ai = arr[i];
        ll v = ans * i + arr[0];
        
        // cout << ai << " " << v << "\n";
        // cout << (ai / rem - (ai < 0 && ai % rem ? 1 : 0)) << " " << (v / rem - (v < 0 && v % rem ? 1 : 0)) << "\n";
        
        if ((ai / rem - (ai < 0 && ai % rem ? 1 : 0)) != (v / rem - (v < 0 && v % rem ? 1 : 0))) {
            valid = false;
            break;
        }
    }
    
    if (valid) {
        cout << fixed;
        cout.precision(9);
        cout << (long double) ans / 1e11;
    }
    else {
        cout << "-1";
    }
}
