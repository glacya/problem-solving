// BOJ 1238 - 파티
// 난이도: Gold III
// 푼 날짜: 2021-07-15
// https://www.acmicpc.net/problem/1238

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <cmath>
#include <string>
#include <cassert>

#pragma warning(disable : 4996)

#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

typedef long long ll;

int MAX(int a, int b) {
	if (a < b) return b;
	else return a;
}

struct comp {
	bool operator()(pair<int, int> p1, pair<int, int> p2) {
		return p1.second > p2.second;
	}
};

void dijkstr(vector<int>& dist, vector<unordered_map<int, int>>& edge, int vertex) {
	priority_queue<pair<int, int>, vector<pair<int, int>>, comp> pq;
	pq.push(make_pair(vertex, 0));
	dist[vertex] = 0;

	while (!pq.empty()) {
		int t = pq.top().first;
		pq.pop();

		for (auto x : edge[t]) {
			if (dist[x.first] > dist[t] + x.second) {
				dist[x.first] = dist[t] + x.second;
				pq.push(make_pair(x.first, dist[x.first]));
			}
		}
	}

}

int main(void) {
	cheatkey;

	int N, M, X;
	cin >> N >> M >> X;

	vector<unordered_map<int, int>> edge1(N + 1);
	vector<unordered_map<int, int>> edge2(N + 1);
	vector<int> dist1(N + 1, 0x7fffffff);
	vector<int> dist2(N + 1, 0x7fffffff);

	for (int i = 0; i < M; i++) {
		int u, v, w;
		cin >> u >> v >> w;

		edge1[u][v] = w;
		edge2[v][u] = w;
	}

	dijkstr(dist1, edge1, X);
	dijkstr(dist2, edge2, X);
	
	int ans = 0;
	for (int i = 1; i <= N; i++) {
		ans = MAX(ans, dist1[i] + dist2[i]);
	}
	cout << ans;

	return 0;
}
