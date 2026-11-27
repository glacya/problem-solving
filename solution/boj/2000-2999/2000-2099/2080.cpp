// BOJ 2080 - 겹치는 선분
// 난이도: Platinum II
// 푼 날짜: 2023-06-10
// https://www.acmicpc.net/problem/2080

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>

using namespace std;
typedef long long ll;
class Segment {
    public:
    ll v1;
    ll v2;
    
    Segment() {}
    Segment(ll v1, ll v2) {
        this->v1 = v1;
        this->v2 = v2;
    }
};

class SegHash {
public:
    ll dx;
    ll dy;
    ll v;
    
    SegHash() {}
    SegHash(ll dx, ll dy, ll v) {
        this->dx = dx;
        this->dy = dy;
        this->v = v;
    }
    
    bool operator<(const SegHash &s1) const {
        if (this->dx == s1.dx && this->dy == s1.dy) {
            return this->v < s1.v;
        }
        
        return this->dy * s1.dx < this->dx * s1.dy;
    }
};

struct cx1 {
    bool operator()(const Segment &s1, const Segment &s2) const {
        if (s1.v1 == s2.v1) return s1.v2 < s2.v2;
        return s1.v1 < s2.v1;
    }
};

struct cx2 {
    bool operator()(const Segment &s1, const Segment &s2) const {
        if (s1.v2 == s2.v2) return s1.v1 < s2.v1;
        return s1.v2 > s2.v2;
    }
};

ll gcd(ll a, ll b) {
    if (a == b) return a;
    ll c = max(a, b);
    ll d = min(a, b);
    
    if (d == 1) return 1;
    if (d == 0) return c;
    
    return gcd(d, c % d);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int n;
    cin >> n;
    
    map<SegHash, pair<vector<Segment>, vector<Segment>>> segments;
    
    for (int i = 0; i < n; i++) {
        ll x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        if (x1 > x2) {
            ll tx = x1;
            ll ty = y1;
            x1 = x2;
            x2 = tx;
            y1 = y2;
            y2 = ty;
        }
        else if (x1 == x2 && y1 > y2) {
            ll ty = y1;
            y1 = y2;
            y2 = ty;
        }
        
        ll dy = y2 - y1;
        ll dx = x2 - x1;
        
        SegHash key;
        Segment new_seg;
        
        float zero = 0.0;
        
        if (dx == 0) {
            key = SegHash(0, 1, x1);
            new_seg = Segment(y1, y2);
        }
        else if (dy == 0) {
            key = SegHash(1, 0, y1);
            new_seg = Segment(x1, x2);
        }
        else {
            ll g = gcd(abs(dy), dx);
            dx /= g;
            dy /= g;
            key = SegHash(dx, dy, x1 * dy - y1 * dx);
            
            new_seg = Segment(x1, x2);
        }
        
        if (segments.find(key) == segments.end()) {
            segments[key] = make_pair(vector<Segment>(), vector<Segment>());
        }
        segments[key].first.push_back(new_seg);
        segments[key].second.push_back(new_seg);
    }
    
    ll total = 0;
    for (auto &p: segments) {
        SegHash key = p.first;
        auto &segvecs = p.second;
        
        ll seg_size = segvecs.first.size() - 1;
        
        sort(segvecs.first.begin(), segvecs.first.end(), cx1());
        sort(segvecs.second.begin(), segvecs.second.end(), cx2());
        
        // cout << "Segments:\n";
        // cout << "Vector 1:\n";
        
        for (Segment &s: segvecs.first) {
            auto l1 = [](Segment lhs, Segment rhs) -> bool { return lhs.v1 < rhs.v2; };
            auto it = lower_bound(segvecs.first.begin(), segvecs.first.end(), s, l1);
            auto d = distance(it, segvecs.first.end());
            // cout << s.v1 << " " << s.v2 << ": " << d << "\n";
            total += seg_size - d;
        }
        // cout << "Vector 2:\n";
        
        for (Segment &s: segvecs.second) {
            auto l2 = [](Segment lhs, Segment rhs) -> bool { return lhs.v2 > rhs.v1; };
            auto it = lower_bound(segvecs.second.begin(), segvecs.second.end(), s, l2);
            auto d = distance(it, segvecs.second.end());
            // cout << s.v1 << " " << s.v2 << ": " << d << "\n";
            total -= d;
        }
    }
    
    cout << total / 2;

    return 0;
}
