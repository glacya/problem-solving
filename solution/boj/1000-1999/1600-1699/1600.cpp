// BOJ 1600 - 말이 되고픈 원숭이
// 난이도: Gold III
// 푼 날짜: 2023-11-01
// https://www.acmicpc.net/problem/1600

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

class Node {
public:
    int x;
    int y;
    int jumps;
    int moves;
    
    Node() {}
    Node(int x, int y, int j, int moves) {
        this->x = x;
        this->y = y;
        this->jumps = j;
        this->moves = moves;
    }
};

vector<int> dx = {0, 0, 1, -1};
vector<int> dy = {1, -1, 0, 0};
vector<int> jdx = {1, 1, 2, 2, -1, -1, -2, -2};
vector<int> jdy = {2, -2, 1, -1, 2, -2, 1, -1};

int main() {
    fastio;
    
    int k, w, h;
    cin >> k >> w >> h;
    
    vector<vector<vector<bool>>> visited(w, vector<vector<bool>>(h, vector<bool>(k + 1)));
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int x;
            cin >> x;
            
            for (int p = 0; p <= k; p++) {
                if (x)
                    visited[j][i][p] = true;
            }
        }
    }
    
    queue<Node> q;
    q.push(Node(0, 0, 0, 0));
    visited[0][0][0] = true;
    
    int min_jump = 1e9;
    while (!q.empty()) {
        Node node = q.front();
        q.pop();
        
        int x = node.x;
        int y = node.y;
        int jump = node.jumps;
        int d = node.moves;
        
        if (x == w - 1 && y == h - 1) {
            min_jump = min(d, min_jump);
            
            continue;
        }
        
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if (nx >= 0 && nx < w && ny >= 0 && ny < h && !visited[nx][ny][jump]) {
                visited[nx][ny][jump] = true;
                q.push(Node(nx, ny, jump, d + 1));
            }
        }
        
        if (jump < k) {
            for (int i = 0; i < 8; i++) {
                int nx = x + jdx[i];
                int ny = y + jdy[i];
                
                if (nx >= 0 && nx < w && ny >= 0 && ny < h && !visited[nx][ny][jump + 1]) {
                    visited[nx][ny][jump + 1] = true;
                    q.push(Node(nx, ny, jump + 1, d + 1));
                }
            }
        }
    }
    
    if (min_jump == 1e9) cout << -1;
    else cout << min_jump;
}
