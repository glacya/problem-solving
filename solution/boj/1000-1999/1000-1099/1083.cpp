// BOJ 1083 - 소트
// 난이도: Gold IV
// 푼 날짜: 2023-08-31
// https://www.acmicpc.net/problem/1083

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    vector<int> arr(n);
    
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    int s;
    cin >> s;
    
    int start = 0;
    
    while (s > 0 && start < n) {
        int max_index = start;
        int local_s = 0;
        
        for (int i = start + 1; i < n && local_s < s; i++) {
            if (arr[max_index] < arr[i]) {
                max_index = i;
            }
            local_s++;
        }
        
        for (int i = max_index; i > start; i--) {
            int temp = arr[i];
            arr[i] = arr[i - 1];
            arr[i - 1] = temp;
            s--;
        }
        
        start++;
    }
    
    for (int i: arr) {
        cout << i << " ";
    }
    
    return 0;
}
