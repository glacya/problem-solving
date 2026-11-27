// BOJ 1987 - 알파벳
// 난이도: Gold IV
// 푼 날짜: 2023-06-07
// https://www.acmicpc.net/problem/1987

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>

using namespace std;

class Node {
public:
    int x;
    int y;
    int s;
    
    Node(){}
    Node(int x, int y, int s) {
        this->x = x;
        this->y = y;
        this->s = s;
    }
};

int bitvec(char c) {
    int x = 1;
    return x << (c - 'A');
}

int size(int n) {
    int ans = 0;
    while (n) {
        if (n % 2) ans++;
        n /= 2;
    }
    return ans;
}

int score = 0;

void dfs(vector<string> &board, int x, int y, int bitv) {
    if (bitvec(board[x][y]) & bitv) return;
    
    int n = board.size();
    int m = board[0].size();
    score = max(score, size(bitv));
    
    int nv = bitvec(board[x][y]);
    
    // cout << hex << bitv << "\n";
    
    if (x > 0) {
        dfs(board, x - 1, y, bitv + nv);
    }
    
    if (x < n - 1) {
        dfs(board, x + 1, y, bitv + nv);
    }
    
    if (y > 0) {
        dfs(board, x, y - 1, bitv + nv);
    }
    
    if (y < m - 1) {
        dfs(board, x, y + 1, bitv + nv);
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int n, m;
    cin >> n >> m;
    
    vector<string> board(n);
    
    for (int i = 0; i < n; i++) {
        cin >> board[i];
    }
    
    dfs(board, 0, 0, 0);
    
    cout << score + 1;

    return 0;
}
