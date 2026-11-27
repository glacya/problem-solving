// BOJ 2162 - 선분 그룹
// 난이도: Platinum V
// 푼 날짜: 2023-04-21
// https://www.acmicpc.net/problem/2162

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;
typedef long long ll;

class Segment {
public:
    ll x1 = 0;
    ll y1 = 0;
    ll x2 = 0;
    ll y2 = 0;
    
    Segment() {}
    
    Segment(ll x1, ll y1, ll x2, ll y2) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    }
};

bool cross(Segment s1, Segment s2) {
    ll x1, y1, x2, y2, x3, y3, x4, y4;
    x1 = s1.x1;
    x2 = s1.x2;
    x3 = s2.x1;
    x4 = s2.x2;
    y1 = s1.y1;
    y2 = s1.y2;
    y3 = s2.y1;
    y4 = s2.y2;
    
    bool answer = false;
    
    // Pass 1: Think about the rectangles, where their diagonal lines are given line segments, respectively.
    // Then, check if the rectangles have intersection. If they have no intersection, the line segments cannot cross.
    auto l1xp = minmax(x1, x2);
    auto l1yp = minmax(y1, y2);
    auto l2xp = minmax(x3, x4);
    auto l2yp = minmax(y3, y4);
    
    bool x_cross = (l2xp.second >= l1xp.first && l1xp.second >= l2xp.first);
    bool y_cross = (l2yp.second >= l1yp.first && l1yp.second >= l2yp.first);
    
    // Pass 2: Now, for each line segment, check if the endpoints of another line segment lie on the different side of the initial segment.
    // This is done by translating the endpoints by the direction parallel to the initial segment, and compare the y-coordinate of them.
    if (x_cross && y_cross) {
        ll dx1 = x2 - x1;
        ll comp1 = y1 * dx1;
        ll trans_y3 = (x1 - x3) * (y2 - y1) + y3 * dx1;
        ll trans_y4 = (x1 - x4) * (y2 - y1) + y4 * dx1;
        bool l2_phase_cross = !((trans_y3 > comp1 && trans_y4 > comp1) || 
                                (trans_y3 < comp1 && trans_y4 < comp1));
        
        ll dx2 = x4 - x3;
        ll comp2 = y3 * dx2;
        ll trans_y1 = (x3 - x1) * (y4 - y3) + y1 * dx2;
        ll trans_y2 = (x3 - x2) * (y4 - y3) + y2 * dx2;
        bool l1_phase_cross = !((trans_y1 > comp2 && trans_y2 > comp2) || 
                                (trans_y1 < comp2 && trans_y2 < comp2));
        
        answer = l2_phase_cross && l1_phase_cross;
    }
    return answer;
}

int find_parent(vector<int> &parent, int index) {
    if (parent[index] == index) return index;
    else return parent[index] = find_parent(parent, parent[index]);
}

void union_parent(vector<int> &parent, int target, int new_friend) {
    int new_parent = find_parent(parent, new_friend);
    int old_parent = find_parent(parent, target);
    
    parent[old_parent] = new_parent;
    find_parent(parent, target);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int N;
    cin >> N;
    
    vector<int> parent(N, -1);
    vector<Segment> segments(N);
    
    for (int i = 0; i < N; i++) {
        ll x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        Segment s(x1, y1, x2, y2);
        parent[i] = i;
        segments[i] = s;
        
        for (int j = 0; j < i; j++) {
            if (cross(segments[j], segments[i])) {
                union_parent(parent, i, j);
            }
        }
    }
    
    unordered_map<int, int> groups;
    int maximum_group_size = 0;
    for (int i = 0; i < N; i++) {
        int value;
        int p = find_parent(parent, i);
        if (groups.find(p) == groups.end()) {
            value = 1;
        }
        else {
            value = groups[p] + 1;
        }
        groups[p] = value;
        maximum_group_size = max(value, maximum_group_size);
    }
    
    cout << groups.size() << "\n" << maximum_group_size;
    
    return 0;
}
