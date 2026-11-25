// BOJ 1131 - 숫자
// 난이도: Platinum V
// 푼 날짜: 2023-05-07
// https://www.acmicpc.net/problem/1131

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;
typedef long long ll;

int s(int n, int k) {
    int f = n;
    int out = 0;
    while (f > 0) {
        int d = 1;
        for (int i = 0; i < k; i++) {
            d *= f % 10;
        }
        out += d;
        f /= 10;
    }
    return out;
}

int memo_min(unordered_map<int, int> &next, unordered_map<int, int> &cmin, int current, int target) {
    if (current == target) return cmin[target];
    
    if (cmin.find(current) != cmin.end()) {
        return cmin[current];
    }
    
    int deeper_min = memo_min(next, cmin, next[current], target);
    return cmin[current] = min(current, deeper_min);
}

int main() {
    int a, b, k;
    cin >> a >> b >> k;
    
    unordered_map<int, int> next;
    unordered_map<int, int> cycle_min;
    
    ll answer = 0;
    for (int i = a; i <= b; i++) {
        // Start from i.
        // cout << "Turn i = " << i << "\n";
        if (cycle_min.find(i) != cycle_min.end()) {
            answer += (ll) cycle_min[i];
            continue;
        }

        int v = i;
        while (next.find(v) == next.end()) {
            int nv = s(v, k);
            next[v] = nv;
            v = nv;
            // cout << "nv = " << v << endl;
        }
        v = next[v];
        // cout << "Found visited node: " << v << "\n";

        // Oh, found cycle. Query on cycle_min if the cycle has already discovered.
        int cycle_start = v;
        if (cycle_min.find(v) == cycle_min.end()) {
            // Not found. Save a new cycle.
            vector<int> entries(1, v);
            int min_num = v;
            bool new_cycle = true;
            while (next[v] != cycle_start) {
                v = next[v];
                entries.push_back(v);
                min_num = min(v, min_num);
                
                if (cycle_min.find(v) != cycle_min.end()) {
                    // cout << "Found existing cycle\n";
                    cycle_start = v;
                    new_cycle = false;
                    break;
                }
            }
            if (new_cycle) {
                for (int e: entries) {
                    // cout << "Saved " << e << "\n";
                    cycle_min[e] = min_num;
                }                
            }
        }
        
        answer += (ll) memo_min(next, cycle_min, i, cycle_start);
    }
    
    cout << answer;

    return 0;
}
