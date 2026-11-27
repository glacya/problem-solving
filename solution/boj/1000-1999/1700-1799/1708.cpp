// BOJ 1708 - 볼록 껍질
// 난이도: Platinum V
// 푼 날짜: 2023-12-11
// https://www.acmicpc.net/problem/1708

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

class Vec {
public:
    ll x;
    ll y;
    
    Vec() {}
    Vec(ll x, ll y) {
        this->x = x;
        this->y = y;
    }
    
    Vec operator+(const Vec &v) const {
        return Vec(this->x + v.x, this->y + v.y);
    }
    
    Vec operator-(const Vec &v) const {
        return Vec(this->x - v.x, this->y - v.y);
    }
    
    bool operator<(const Vec &v) const {
        ll c = this->x * v.y - this->y * v.x;
        
        if (c == 0) {
            if (this->y == v.y) return abs(this->x) > v.x;
            return abs(this->y) > abs(v.y);
        }
        else return c > 0;
    }
};

ll cross(Vec v1, Vec v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

int main() {
    fastio;
    
    int n;
    cin >> n;
    
    Vec start(1e18, 1e18);
    vector<Vec> points(n);
    
    for (int i = 0; i < n; i++) {
        ll x, y;
        cin >> x >> y;
        
        points[i] = Vec(x, y);
        
        if (x < start.x) start = points[i];
        else if (x == start.x && y < start.y) start = points[i];
    }
    
    for (int i = 0; i < n; i++) {
        points[i] = points[i] - start;
    }
    
    sort(points.begin(), points.end());
    
    stack<Vec> st;
    st.push(points[0]);
    st.push(points[1]);
    
    for (int i = 2; i < n; i++) {
        while (st.size() >= 2) {
            Vec p1 = st.top();
            st.pop();
            
            Vec p0 = st.top();
            Vec p2 = points[i];
            Vec l1 = p1 - p0;
            Vec l2 = p2 - p1;
            
            if (cross(l1, l2) > 0) {
                st.push(p1);
                break;
            }
        }
        st.push(points[i]);
    }
    
    // while (!st.empty()) {
    //     cout << st.top().x << " " << st.top().y << "\n";
    //     st.pop();
    // }
    
    cout << st.size();
}
