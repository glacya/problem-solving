// BOJ 1019 - 책 페이지
// 난이도: Platinum V
// 푼 날짜: 2023-12-06
// https://www.acmicpc.net/problem/1019

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

vector<int> rec(int n, int d, bool is_sig) {
    vector<int> result(10);
    // cout << "rec(" << n << ", " << d << ", " << is_sig << ")\n";
    
    if (d == 1) {
        for (int i = 0; i <= n; i++) {
            result[i] = 1;
        }
        
        if (is_sig) result[0] = 0;
        
        return result;
    }
    
    int x = n;
    int w = 1;
    while (x >= 10) {
        x /= 10;
        w *= 10;
    }
    
    int largest = x * w;
    int k = n - largest;
    
    vector<int> vtrue = rec(w - 1, d - 1, true);
    vector<int> vfalse = rec(w - 1, d - 1, false);
    
    for (int i = 0; i < x; i++) {
        if (is_sig && (i == 0)) {
            for (int j = 0; j < 10; j++) {
                result[j] += vtrue[j];
            }
        }
        else {
            for (int j = 0; j < 10; j++) {
                result[j] += vfalse[j];
            }
        }
        
        if (!is_sig || i > 0) {
            result[i] += w;
        } 
    }
    
    vector<int> rem = rec(k, d - 1, false);
    
    for (int j = 0; j < 10; j++) {
        result[j] += rem[j];
    }
    
    result[x] += k + 1;
    
    return result;
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    int d = 0;
    int x = n;
    while (x) {
        d++;
        x /= 10;
    }
    
    vector<int> v = rec(n, d, true);
    
    for (int i = 0; i < 10; i++) {
        cout << v[i] << " ";
    }
}
