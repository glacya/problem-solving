// BOJ 1332 - 풀자
// 난이도: Gold III
// 푼 날짜: 2023-09-29
// https://www.acmicpc.net/problem/1332

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, v;
    cin >> n >> v;
    
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    int min_prob = n;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (abs(arr[i] - arr[j]) >= v) {
                min_prob = min(min_prob, (i + 1) / 2 + (j - i + 1) / 2 + 1);
            }
        }
    }
    
    cout << min_prob;
}
