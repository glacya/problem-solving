// BOJ 22899 - 오렌지컵 출제하기
// 난이도: Gold II
// 푼 날짜: 2026-11-24
// https://www.acmicpc.net/problem/22899

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

// 22899
// Pair: (Time, SetterId)

struct Comparator {
    bool operator()(const pair<ll, ll> &p1, const pair<ll, ll> &p2) const {
        if (p1.first == p2.first) {
            return p1.second < p2.second;
        }

        return p1.first < p2.first;
    }
};

bool sort_pair_by_setter(pair<ll, ll> &p1, pair<ll, ll> &p2) {
    if (p1.second == p2.second) {
        return p1.first < p2.first;
    }

    return p1.second < p2.second;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    // Pair: (Time, SetterId)
    vector<pair<ll, ll>> problems(n);
    vector<vector<pair<ll, ll>>> problem_slots(n);
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, Comparator> pq;

    for (int i = 0; i < n; i++) {
        cin >> problems[i].second;
    }

    for (int i = 0; i < n; i++) {
        cin >> problems[i].first;
    }

    // Sort by setter.
    sort(problems.begin(), problems.end(), sort_pair_by_setter);

    // If a setter proposes multiple problems, the problems are put into separate slots.
    // Problem with less preparation time goes into lower-index slot.
    int prev_setter = -1;
    int slot = 0;

    for (int i = 0; i < n; i++) {
        if (problems[i].second != prev_setter) {
            slot = 0;
            prev_setter = problems[i].second;
        }

        problem_slots[slot++].push_back(problems[i]);
    }

    // for (int i = 0; i < n; i++) {
    //     for (auto &p: problem_slots[i]) {
    //         cout << "(" << p.first << ", " << p.second << ") ";
    //     }

    //     cout << "\n";
    // }

    ll preparation_time = 0;

    // Now, compute from L = 1 to L = N.
    for (int l = 1; l <= n; l++) {
        int slot = l - 1;

        // Push every problem of problem_slots[i] into PQ, and pull out exceeding elements.
        for (auto &problem: problem_slots[slot]) {
            pq.push(problem);
            preparation_time += problem.first;
        }
        
        // If the size of PQ is under k, then it is infeasible.
        if (pq.size() < k) {
            cout << "-1 ";
            continue;
        }

        // If the size of PQ is over k, then we should eliminate some problems.
        while (pq.size() > k) {
            pair<ll, ll> data = pq.top();
            preparation_time -= data.first;

            pq.pop();
        }

        // At this point, the size of PQ should be exactly K.
        cout << preparation_time << " ";
    }

    return 0;
}
