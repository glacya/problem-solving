// BOJ 1508 - 레이스
// 난이도: Gold II
// 푼 날짜: 2023-10-19
// https://www.acmicpc.net/problem/1508

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, m, k;
    cin >> n >> m >> k;
    
    vector<int> arr(k);
    
    for (int i = 0; i < k; i++) cin >> arr[i];
    
    int left = 0;
    int right = 1000001;
    
    vector<bool> place(k);
    while (left + 1 < right) {
        int mid = (left + right) / 2;
        int count = 0;
        int lv = -1e8;
        
        place = vector<bool>(k);
        
        for (int i = 0; i < k; i++) {
            if (arr[i] - lv >= mid) {
                place[i] = true;
                count++;
                lv = arr[i];
            }
            
            if (count == m) break;
        }
        
        if (count < m) {
            // Too large interval
            right = mid;
        }
        else {
            // Too small interval
            left = mid;
        }
    }
    
    place = vector<bool>(k);
    
    int lv = -1e8;
    int count = 0;
    for (int i = 0; i < k; i++) {
        if (arr[i] - lv >= left && count < m) {
            cout << "1";
            lv = arr[i];
            count++;
        }
        else cout << "0";
    }
}
