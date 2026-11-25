// BOJ 1043 - 거짓말
// 난이도: Gold IV
// 푼 날짜: 2023-03-13
// https://www.acmicpc.net/problem/1043

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    int S;
    queue<int> truth_queue;
    cin >> S;
    
    for (int i = 0; i < S; i++) {
        int x;
        cin >> x;
        truth_queue.push(x - 1);
    }
    
    vector<vector<bool>> connected(N, vector<bool>(N, false));
    vector<bool> visited(N, false);
    
    vector<vector<int>> parties;
    int safe_parties = 0;
    
    for (int i = 0; i < M; i++) {
        int party_count;
        cin >> party_count;
        vector<int> party_info;
        
        for (int j = 0; j < party_count; j++) {
            int x;
            cin >> x;
            party_info.push_back(--x);
            for (int info: party_info) {
                connected[x][info] = true;
                connected[info][x] = true;
            }
        }
        
        parties.push_back(party_info);
    }
    
    while (!truth_queue.empty()) {
        int person = truth_queue.front();
        truth_queue.pop();
        
        if (visited[person]) continue;
        visited[person] = true;
        
        for (int i = 0; i < N; i++) {
            if (connected[person][i]) {
                truth_queue.push(i);
            }
        }
    }
    
    for (auto party: parties) {
        bool safe = true;
        for (int x: party) {
            if (visited[x]) {
                safe = false;
                break;
            }
        }
        if (safe) safe_parties++;
    }
    
    cout << safe_parties;

    return 0;
}
