// BOJ 1799 - 비숍
// 난이도: Platinum V
// 푼 날짜: 2023-05-27
// https://www.acmicpc.net/problem/1799

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int track_cases(vector<vector<bool>> &board, vector<pair<int, int>> &bishops, int r, int c, int n) {
    if (r == n) return bishops.size();
    
    int next_r = r + (c + 2) / n;
    int next_c = (c + 2) % n;
    
    if (c + 2 >= n && n % 2 == 0) next_c = (next_c == 0) ? 1 : 0;
    
    int cases = track_cases(board, bishops, next_r, next_c, n);
    if (board[r][c]) {
        bool place_ok = true;
        for (auto &v: bishops) {
            if (v.first + v.second == r + c) {
                place_ok = false;
                break;
            }
            
            if (v.second - v.first == c - r) {
                place_ok = false;
                break;
            }
        }
        
        if (place_ok) {
            vector<pair<int, int>> next_bishops(bishops);
            next_bishops.push_back(make_pair(r, c));
            cases = max(cases, track_cases(board, next_bishops, next_r, next_c, n));
        }
    }
    
    return cases;
}

int main() {
    int n;
    cin >> n;
    
    vector<vector<bool>> board(n, vector<bool>(n));
    vector<pair<int, int>> bishops;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x;
            cin >> x;
            board[i][j] = (x == 1);
        }
    }
    
    int ans = track_cases(board, bishops, 0, 0, n);
    if (n != 1) {
        bishops = vector<pair<int, int>>();
        ans += track_cases(board, bishops, 0, 1, n);
    }
    
    cout << ans;
    
    return 0;
}
