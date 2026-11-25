// BOJ 1132 - 합
// 난이도: Gold III
// 푼 날짜: 2023-06-30
// https://www.acmicpc.net/problem/1132

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
typedef long long ll;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int N;
    cin >> N;

    vector<ll> weights(10, 0);
    vector<bool> no_zero(10, 0);
    
    for (int i = 0; i < N; i++) {
        string input;
        cin >> input;
        
        ll weight = 1;
        for (int j = input.size() - 1; j >= 0; j--) {
            weights[input[j] - 'A'] += weight;
            weight *= 10;
            
            if (j == 0) {
                no_zero[input[j] - 'A'] = true;
                // cout << input[j] << " is marked as nonzero\n";
            }
        }
    }
    
    ll zero_candi = 10000000000000;
    ll zero_index = -1;
    for (int i = 0; i < 10; i++) {
        // cout << "weights[" << (char) (i + 'A') << "] = " << weights[i] << "\n";
        if (!no_zero[i] && weights[i] < zero_candi) {
            zero_index = i;
            zero_candi = weights[i];
        }   
    }
    if (zero_index >= 0) weights[zero_index] = 0;
    
    sort(weights.begin(), weights.end());
    
    ll total = 0;
    for (int i = 0; i < 10; i++) {
        if (weights[i] == 0) {
            continue;
        }

        total += i * weights[i];
    }
    cout << total;
    
    return 0;
}
