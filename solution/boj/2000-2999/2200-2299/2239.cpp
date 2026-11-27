// BOJ 2239 - 스도쿠
// 난이도: Gold IV
// 푼 날짜: 2023-06-02
// https://www.acmicpc.net/problem/2239

#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<vector<int>> backtrack(vector<vector<int>> &sudoku, int r, int c) {
    // cout << "Backtrack " << r << " " << c << "\n";
    if (r == 9) return sudoku;
    
    int next_r = r + (c + 1) / 9;
    int next_c = (c + 1) % 9;
    
    if (sudoku[r][c] != 0) return backtrack(sudoku, next_r, next_c);
    
    vector<vector<int>> middle_board;
    
    for (int i = 1; i <= 9; i++) {
        bool success = true;
        for (int j = 0; j < 9; j++) {
            if (sudoku[r][j] == i) {
                success = false;
                break;
            }
        }
        
        for (int j = 0; j < 9; j++) {
            if (sudoku[j][c] == i) {
                success = false;
                break;
            }
        }
        
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (sudoku[(r / 3) * 3 + j][(c / 3) * 3 + k] == i) {
                    success = false;
                    break;
                }
            }
            if (!success) break;
        }
        
        if (success) {
            vector<vector<int>> next_sudoku(sudoku);
            next_sudoku[r][c] = i;
            middle_board = backtrack(next_sudoku, next_r, next_c);
            
            if (middle_board.size() > 0) break;
        }
    }
    return middle_board;
}

int main()
{
    vector<vector<int>> sudoku(9, vector<int>(9));
    for (int i = 0; i < 9; i++) {
        string row;
        cin >> row;
        
        for (int j = 0; j < 9; j++) {
            char c = row[j];
            sudoku[i][j] = c - '0';
        }
    }
    
    sudoku = backtrack(sudoku, 0, 0);
    
    for (auto v: sudoku) {
        for (int x: v) {
            cout << x;
        }
        cout << "\n";
    }

    return 0;
}
