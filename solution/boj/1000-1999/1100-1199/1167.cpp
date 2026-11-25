// BOJ 1167 - 트리의 지름
// 난이도: Gold II
// 푼 날짜: 2021-06-21
// https://www.acmicpc.net/problem/1167

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <queue>

#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

typedef long long ll;

ll match(vector<pair<int, ll>>& v, int query) {
	if (query == 0) return 0;
	for (const auto& x : v) {
		if (x.first == query) return x.second;
	}
	return -1;
}

ll MAX(ll a, ll b) {
	if (a < b) return b;
	else return a;
}

pair<ll, ll> DFS(vector<vector<pair<int, ll>>>& v, int vertex, int prev) {		// (The largest radius of subtree, The deepest depth)
	ll max1 = 0;
	ll max2 = 0;
	ll maxlocal = 0;
	ll m = match(v[vertex], prev);
	//cout << "DFS(vertex = " << vertex << ", prev = " << prev << ") is called\n";
	for (const auto& x : v[vertex]) {
		if (x.first != prev) {
			auto dfs = DFS(v, x.first, vertex);
			if (dfs.first > maxlocal) maxlocal = dfs.first;
			if (dfs.second > max1) {
				max2 = max1;
				max1 = dfs.second;
			}
			else if (dfs.second > max2) {
				max2 = dfs.second;
			}
		}
	}
	//cout << "\tDFS(vertex = " << vertex << ", prev = " << prev << ") has value max1 = " << max1;
	//cout << ", max2 = " << max2 << ", maxlocal = " << maxlocal << ", pair<" << MAX(maxlocal, max1 + max2);
	//cout << ", " << max1 + m << ">" << endl;
	return make_pair(MAX(maxlocal, max1 + max2), max1 + m);
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
	int N;
	cin >> N;
	queue<int> q;
	vector<vector<pair<int, ll>>> v = vector<vector<pair<int,ll>>>(N + 1, vector<pair<int, ll>>());

	for (int i = 0; i < N; i++) {
		int host;
		cin >> host;
		while (1) {
			int vertex;
			ll weight;
			cin >> vertex;
			if (vertex == -1) break;
			cin >> weight;
			v[host].push_back(make_pair(vertex,weight));
		}
	}
	cout << DFS(v, 1, 0).first;

	return 0;
}
