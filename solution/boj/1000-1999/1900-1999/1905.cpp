// BOJ 1905 - 상자 쌓기
// 난이도: Gold III
// 푼 날짜: 2023-09-13
// https://www.acmicpc.net/problem/1905

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

class Box {
public:
    int z;
    int x;
    int y;
    int w;
    int h;
    
    Box(){}
    Box(int z, int x, int y, int w, int h) {
        this->z = z;
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    
    bool operator<(const Box &b) const {
        if (this->z == b.z) {
            if (this->x == b.x) return this->y > b.y;
            return this->x > b.x;
        }
        
        return this->z > b.z;
    }
};

int main() {
    fastio;
    
    int lx, ly, n;
    cin >> lx >> ly >> n;
    
    set<Box> boxes;
    
    for (int i = 0; i < n; i++) {
        int w, h, x, y, z;
        cin >> w >> h >> z >> x >> y;
        
        for (Box b: boxes) {
            // cout << b.z << " ";
            if (x < b.x + b.w && x + w > b.x && y < b.y + b.h && y + h > b.y) {
                // cout << "\nFound overlapping box with height " << b.z << "\n";
                // cout << "\t" << boxes.size() << "\n";
                z += b.z;
                break;
            }
            // cout << "\n";
        }
        
        boxes.insert(Box(z, x, y, w, h));
    }
    
    cout << boxes.begin()->z;
    
    return 0;
}
