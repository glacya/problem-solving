// BOJ 1399 - 보물의 위치
// 난이도: Platinum V
// 푼 날짜: 2023-11-03
// https://www.acmicpc.net/problem/1399

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

class Point {
public:
    ll x = 0;
    ll y = 0;
    
    Point() {
        this->x = 0;
        this->y = 0;
    }
    
    void move(ll v, int kind) {
        switch (kind % 4) {
            case 0:
                this->y += v;
                break;
            case 1:
                this->x += v;
                break;
            case 2:
                this->y -= v;
                break;
            case 3:
                this->x -= v;
                break;
        }
    }
};

int main() {
    fastio;
    
    int t;
    cin >> t;
    
    vector<vector<ll>> moves = {
        {1},
        {1, 2, 4, 8, 7, 5},
        {9, 9, 9, 9},
        {1, 4, 7, 1, 4, 7},
        {1, 5, 7, 8, 4, 2},
        {9},
        {1, 7, 4, 1, 7, 4},
        {1, 8, 1, 8, 1, 8},
        {9}
    };
    
    while (t--) {
        ll k, m;
        cin >> k >> m;
        
        ll type = (m - 1) % 9;
        
        Point point;
        point.x = 0;
        point.y = 0;
        
        switch (type) {
            case 0: {
                for (int i = 0; i < k % 4; i++) {
                    point.move(moves[type][0], i);
                }
                break;
            }
            case 1:
            case 3:
            case 4:
            case 6: 
            case 7: {
                for (int i = 0; i < k % 12; i++) {
                    point.move(moves[type][i % 6], i % 4);
                }
                break;
            }
            case 2:
            case 5:
            case 8: {
                if (k == 1) {
                    point.x = 0;
                    point.y = 1;
                    break;
                }
                
                if (k == 2) {
                    point.x = type + 1;
                    point.y = 1;
                    break;
                }
                
                point.x = type + 1;
                point.y = 1;
                
                for (int i = 0; i < (k + 2) % 4; i++) {
                    point.move(9, (i + 2) % 4);
                }
            }
        }
        
        cout << point.x << " " << point.y << "\n";
    }
    
   
}
