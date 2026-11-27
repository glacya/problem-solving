// BOJ 2042 - 구간 합 구하기
// 난이도: Gold I
// 푼 날짜: 2023-04-15
// https://www.acmicpc.net/problem/2042

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

ll element_start = 0;

void change_value(vector<ll> &segtree, ll index, ll value) {
    // Be sure to accept index subtracted by 1 from the input.
    ll old_value = segtree[element_start + index];
    ll delta = value - old_value;
    segtree[element_start + index] = value;
    
    ll rise_index = (element_start + index) / 2;
    while (rise_index > 0) {
        segtree[rise_index] += delta;
        rise_index /= 2;
    }
    return;
}

pair<ll, ll> node_to_range(ll node) {
    ll offset = 0;
    ll width = element_start;
    ll level = node;
    
    while (level > 1) {
        level /= 2;
        width /= 2;
    }
    
    ll step = width;
    while (node > 1) {
        if (node % 2) {
            offset += step;
        }
        node /= 2;
        step *= 2;
    }
    return make_pair(offset, offset + width - 1);
}

ll find_sum(vector<ll> &segtree, ll i1, ll i2, ll node) {
    if (i1 == i2) return segtree[element_start + i1];
    
    auto range = node_to_range(node);
    
    // cout << "find_minimum_index(" << i1 << ", " << i2 << ")\n";
    // cout << "node_to_range(" << node << ") = " << range.first << ", " << range.second << "\n";
    
    if (i1 == range.first && i2 == range.second) return segtree[node];
    
    ll split_point = (range.first + range.second + 1) / 2;
    
    if (i1 >= split_point) return find_sum(segtree, i1, i2, node * 2 + 1);
    
    if (i2 < split_point) return find_sum(segtree, i1, i2, node * 2);
    
    ll left_sum = find_sum(segtree, i1, split_point - 1, node * 2);
    ll right_sum = find_sum(segtree, split_point, i2, node * 2 + 1);
    // cout << "left = " << left_minimum_index << ", right = " << right_minimum_index << "\n";
    return left_sum + right_sum;
}

ll upper_power_of_2(ll integer) {
    ll result = 1;
    while (result < integer) {
        result <<= 1;
    }
    return result * 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    ll N, M, K;
    cin >> N >> M >> K;
    
    ll vector_size = upper_power_of_2(N);
    element_start = vector_size / 2;
    
    vector<ll> segtree(vector_size + 1, 0);

    for (ll i = 0; i < N; i++) {
        ll entry;
        cin >> entry;
        change_value(segtree, i, entry);
    }
    

    for (int i = 0; i < M + K; i++) {
        ll querykind, v1, v2;
        cin >> querykind >> v1 >> v2;
        
        if (querykind == 1) {
            change_value(segtree, v1 - 1, v2);
        }
        else {
            cout << find_sum(segtree, v1 - 1, v2 - 1, 1) << "\n";
        }
    }
    
    return 0;
}
