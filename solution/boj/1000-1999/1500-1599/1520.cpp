// BOJ 1520 - 내리막 길
// 난이도: Gold III
// 푼 날짜: 2023-05-23
// https://www.acmicpc.net/problem/1520

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
typedef long long ll;
#define fio ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);

class Node {
public:
    int x = 0;
    int y = 0;
    int h = 0;
    
    Node(){}
    Node(int x, int y, int h) {
        this->x = x;
        this->y = y;
        this->h = h;
    }
    
    bool operator<(const Node n) const {
        return this->h < n.h;
    }
};

int main()
{
    fio;
    
    int M, N;
    cin >> M >> N;
    
    vector<vector<int>> world(M, vector<int>(N));
    vector<vector<int>> ways(M, vector<int>(N));
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> world[i][j];
        }
    }
    
    priority_queue<Node> pq;
    pq.push(Node(0, 0, world[0][0]));
    ways[0][0] = 1;
    
    while (!pq.empty()) {
        auto curr = pq.top();
        pq.pop();
        
        int x = curr.x;
        int y = curr.y;
        int h = world[x][y];
        
        if (h < 0) continue;
        
        // cout << "Point " << x << ", " << y << ": Height " << h << "\n";
        
        if (x > 0 && world[x - 1][y] < h) {
            pq.push(Node(x - 1, y, world[x - 1][y]));
            ways[x - 1][y] += ways[x][y];
        }
        
        if (x < M - 1 && world[x + 1][y] < h) {
            pq.push(Node(x + 1, y, world[x + 1][y]));
            ways[x + 1][y] += ways[x][y];
        }
        
        if (y > 0 && world[x][y - 1] < h) {
            pq.push(Node(x, y - 1, world[x][y - 1]));
            ways[x][y - 1] += ways[x][y];
        }
        
        if (y < N - 1 && world[x][y + 1] < h) {
            pq.push(Node(x, y + 1, world[x][y + 1]));
            ways[x][y + 1] += ways[x][y];
        }
        
        world[x][y] = -1;
    }
    
    cout << ways[M - 1][N - 1];
}
