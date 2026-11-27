// BOJ 2065 - 나룻배
// 난이도: Gold II
// 푼 날짜: 2023-12-27
// https://www.acmicpc.net/problem/2065

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;

class Man {
public:
    int t;
    int index;
    int value = 0;
    bool left;
    
    Man(){}
    Man(int t1, int i1, bool l1): t(t1), index(i1), left(l1) {}
    
    bool operator<(const Man &m) const {
        return this->t < m.t;
    }
};

int main() {
    fastio;
    
    int m, t, n;
    cin >> m >> t >> n;
    
    vector<Man> man(n);
    
    for (int i = 0; i < n; i++) {
        int x;
        string s;
        cin >> x >> s;
        
        man[i] = Man(x, i, s == "left");
    }
    
    vector<int> ans(n);
    
    sort(man.begin(), man.end());
    
    bool at_left = true;
    int boat_time = 0;
    queue<Man> lq, rq;
    int i = 0;
    int ts = 0;
    while (i < n || !lq.empty() || !rq.empty()) {
        while (i < n && man[i].t == ts) {
            // cout << "Push " << man[i].t << ", " << man[i].left << "\n";
            
            if (man[i].left) lq.push(man[i]);
            else rq.push(man[i]);
            i++;
        }
        
        if (ts < boat_time) {
            ts++;
            continue;
        }
        
        if (at_left) {
            if (lq.empty() && !rq.empty()) {
                // cout << "Time " << ts << ": Now at left, detected right.\n";
                boat_time = ts + t;
                at_left = false;
            }
            else if (!lq.empty()) {
                int cap = 0;
                while (!lq.empty() && cap++ < m) {
                    auto mv = lq.front();
                    lq.pop();
                    
                    ans[mv.index] = ts + t;
                }
                boat_time = ts + t;
                at_left = false;
            }
        }
        else {
            if (rq.empty() && !lq.empty()) {
                boat_time = ts + t;
                at_left = true;
            }
            else if (!rq.empty()) {
                int cap = 0;
                while (!rq.empty() && cap++ < m) {
                    auto mv = rq.front();
                    rq.pop();
                    
                    ans[mv.index] = ts + t;
                }
                boat_time = ts + t;
                at_left = true;
            }
        }
        
        // if (ts % 10 == 0) {
        //     cout << lq.size() << " " << rq.size() << "\n";
        // }
        
        ts++;
    }
    
    for (int x: ans) {
        cout << x << "\n";
    }
}
