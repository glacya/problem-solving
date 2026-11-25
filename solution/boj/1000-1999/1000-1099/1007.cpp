// BOJ 1007 - 벡터 매칭
// 난이도: Gold II
// 푼 날짜: 2023-04-15
// https://www.acmicpc.net/problem/1007

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef long long ll;

ll compute_minimum_distance(vector<pair<ll, ll>> &vectors, ll x_sum, ll y_sum, int last_point, int points, int N) {
    if (points == N / 2) {
        return x_sum * x_sum + y_sum * y_sum;
    }
    
    ll minimum_distance = 0x7fffffffffffffff;
    for (int i = last_point + 1; i < N; i++) {
        minimum_distance = min(
            minimum_distance, 
            compute_minimum_distance(
                vectors, 
                x_sum - 2 * vectors[i].first, 
                y_sum - 2 * vectors[i].second, 
                i, 
                points + 1, 
                N
            )
        );
    }
    
    return minimum_distance;
}

int main() {
    int T;
    cin >> T;
    
    cout << fixed;
    cout.precision(8);
    
    for (int i = 0; i < T; i++) {
        vector<pair<ll, ll>> vectors;
        int N;
        cin >> N;
        
        ll x_total = 0;
        ll y_total = 0;
        for (int j = 0; j < N; j++) {
            ll x, y;
            cin >> x >> y;
            x_total += x;
            y_total += y;
            vectors.push_back(make_pair(x, y));
        }
        
        ll min_dist = compute_minimum_distance(vectors, x_total, y_total, -1, 0, N);
        double answer = sqrt((double) min_dist);
        
        cout << answer << "\n";
    }

    return 0;
}
