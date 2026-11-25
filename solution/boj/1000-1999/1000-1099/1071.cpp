// BOJ 1071 - 소트
// 난이도: Platinum V
// 푼 날짜: 2023-05-02
// https://www.acmicpc.net/problem/1071

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>

using namespace std;

vector<int> rec_sort(map<int, int> numbers, vector<int> result, int index, int N) {
    if (index == N) return result;
    
    for (auto p: numbers) {
        if (index == 0 || (p.first != result[index - 1] + 1 && p.second > 0)) {
            map<int, int> new_map(numbers);
            new_map[p.first] = p.second - 1;
            vector<int> new_vec(result);
            new_vec.push_back(p.first);
            
            vector<int> true_result = rec_sort(new_map, new_vec, index + 1, N);
            if (true_result.size() != 0) return true_result;
        }
    }
    return vector<int>();
}

int main() {
    int N;
    cin >> N;
    vector<int> sequence(N);
    
    for (int i = 0; i < N; i++) {
        cin >> sequence[i];
    }
    
    sort(sequence.begin(), sequence.end());

    int lastpoint = 0;
    map<int, int> numbers;
    
    for (int i = 1; i < N; i++) {
        if (sequence[i] != sequence[i - 1]) {
            numbers[sequence[i - 1]] = i - lastpoint;
            lastpoint = i;
        }
    }
    numbers[sequence[N - 1]] = N - lastpoint;
    
    vector<int> result = rec_sort(numbers, vector<int>(), 0, N);
    
    for (int i = 0; i < N; i++) {
        cout << result[i] << " ";
    }

    return 0;
}
