// BOJ 1352 - 문자열
// 난이도: Gold I
// 푼 날짜: 2023-12-16
// https://www.acmicpc.net/problem/1352

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    if (n == 2 || n == 4 || n == 5 || n == 8 || n == 9) {
        cout << -1;
        return 0;
    }
    
    if (n == 1) {
        cout << "A";
        return 0;
    }
    
    vector<int> pos;
    
    int v = 1;
    while (v <= n) {
        pos.push_back(v);
        v = v * 2;
    }
    
    int k = pos.size();
    for (v--; v > n; v--) {
        for (int i = k - 1; i > 0; i--) {
            if (pos[i] != pos[i - 1] + 1) {
                pos[i]--;
                break;
            }
        }
    }
    
    // for (int i = 0; i < k; i++) {
    //     cout << pos[i] << " ";
    // }
    // cout << "\n";
    
    vector<int> nums = pos;
    for (int i = 1; i <= n; i++) {
        bool in_pos = false;
        for (int j = 0; j < k; j++) {
            if (i == pos[j]) {
                nums[j]--;
                cout << (char) (j + 'A');
                in_pos = true;
                break;
            }
        }
        
        if (!in_pos) {
            for (int j = 0; j < k; j++) {
                if (nums[j]) {
                    cout << (char) (j + 'A');
                    nums[j]--;
                    break;
                } 
            }
        }
    }
}
