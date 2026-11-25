// BOJ 1027 - 고층 건물
// 난이도: Gold IV
// 푼 날짜: 2023-06-28
// https://www.acmicpc.net/problem/1027

#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

int main() {
    int N;
    cin >> N;
    
    vector<ll> bs(N);
    
    for (int i = 0; i < N; i++) {
        cin >> bs[i];
    }
    
    ll max_count = 0;
    for (int i = 0; i < N; i++) {
        ll count = 0;
        
        // cout << "At " << i << "-th building\n";
        ll ls_num = 2000000000;
        ll ls_den = 1;
        for (int j = i - 1; j >= 0; j--) {
            ll c_num = bs[i] - bs[j];
            ll c_den = i - j;
            
            if (ls_num * c_den > ls_den * c_num) {
                count++;
                ls_num = c_num;
                ls_den = c_den;
            }
        }
        
        // cout << count << " ";

        ll rs_num = -2000000000;
        ll rs_den = 1;
        for (int j = i + 1; j < N; j++) {
            ll c_num = bs[j] - bs[i];
            ll c_den = j - i;
            if (rs_num * c_den < rs_den * c_num) {
                count++;
                rs_num = c_num;
                rs_den = c_den;
            }
        }
        
        // cout << count << "\n";
        
        max_count = max(max_count, count);
    }
    
    cout << max_count;

    return 0;
}
