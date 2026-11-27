// BOJ 2143 - 두 배열의 합
// 난이도: Gold III
// 푼 날짜: 2023-03-10
// https://www.acmicpc.net/problem/2143

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    int T;
    int n, m;
    
    cin >> T;
    cin >> n;
    
    vector<int> a_sum(1, 0);
    int a_acc = 0;
    
    for (int i = 0; i < n; i++) {
        int elem;
        cin >> elem;
        a_acc += elem;
        a_sum.push_back(a_acc);
    }
    
    cin >> m;
    
    vector<int> b_sum(1, 0);
    int b_acc = 0;
    
    for (int i = 0; i < m; i++) {
        int elem;
        cin >> elem;
        b_acc += elem;
        b_sum.push_back(b_acc);
    }
    
    long long num_of_cases = 0;
    unordered_map<int, int> a_partial_sums;
    
    for (int i = 0; i < n + 1; i++) {
        for (int j = i + 1; j < n + 1; j++) {
            int partial_sum = a_sum[j] - a_sum[i];
            
            if (a_partial_sums.find(partial_sum) == a_partial_sums.end()) {
                a_partial_sums.insert(make_pair(partial_sum, 1));
            }
            else {
                a_partial_sums[partial_sum] += 1;
            }
        }
    }
    
    for (int i = 0; i < m + 1; i++) {
        for (int j = i + 1; j < m + 1; j++) {
            int key = T - b_sum[j] + b_sum[i];
            
            if (a_partial_sums.find(key) != a_partial_sums.end()) {
                num_of_cases += a_partial_sums[key];
            }
        }
    }
    
    cout << num_of_cases;

    return 0;
}
