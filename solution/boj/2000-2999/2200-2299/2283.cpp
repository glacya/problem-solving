// BOJ 2283 - 구간 자르기
// 난이도: Gold II
// 푼 날짜: 2023-10-15
// https://www.acmicpc.net/problem/2283

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n;
    ll k;
    cin >> n >> k;
    
    vector<pair<ll, ll>> segment(n);
    
    auto lambda = [](const pair<ll, ll> &p1, const pair<ll, ll> &p2) -> bool {
        return p1.second > p2.second;
    };
    
    for (int i = 0; i < n; i++) {
        ll u, v;
        cin >> u >> v;
        
        segment[i] = {u, v};
    }
    
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, decltype(lambda)> pq(lambda);
    
    sort(segment.begin(), segment.end());
    
    vector<ll> prefix_sum(1000001);
    int segdex = 0;
    ll x = 0;
    for (; x <= 1000000; x++) {
        if (x > 0)
            prefix_sum[x] = prefix_sum[x - 1] + (ll) pq.size();
        
        while (segdex < n && segment[segdex].first == x) {
            pq.push(segment[segdex]);
            segdex++;
        }
        
        while (!pq.empty() && pq.top().second <= x) {
            pq.pop();
        }
        
        if (segdex == n && pq.empty()) break;
    }
    
    ll left = 0;
    ll right = 0;
    
    while (!(left == x && right == x)) {
        ll value = prefix_sum[right] - prefix_sum[left];
        
        if (value < k) {
            if (right == x) {
                break;
            }
            
            right++;
        }
        else if (value > k) {
            left++;
        }
        else {
            cout << left << " " << right << " ";
            return 0;
        }
    }
    
    cout << "0 0";
}
