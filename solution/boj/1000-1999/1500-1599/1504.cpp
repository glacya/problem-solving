// BOJ 1504 - 특정한 최단 경로
// 난이도: Gold IV
// 푼 날짜: 2022-04-13
// https://www.acmicpc.net/problem/1504

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>
#include <cassert>

#pragma warning(disable : 4996)

#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

typedef long long ll;

struct comp {
	bool operator()(pair<int, int> p1, pair<int, int> p2) {
		return p1.second > p2.second;
	}
};

void dijkstra(vector<int>& dist, vector<vector<int>>& edge, int vertex) {
	int V = edge.size() - 1;

	priority_queue<pair<int, int>, vector<pair<int, int>>, comp> pq;
	pq.push(make_pair(vertex, 0));
	dist[vertex] = 0;

	while (!pq.empty()) {
		auto t = pq.top();
		pq.pop();

		for (int i = 1; i <= V; i++) {
			if ((edge[t.first][i] != 0x7fffffff) && (dist[i] > dist[t.first] + edge[t.first][i])) {
				dist[i] = dist[t.first] + edge[t.first][i];
				pq.push(make_pair(i, dist[i]));
			}
		}
	}

}


int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int INF = 0x7fffffff;
	int V, E;
	cin >> V >> E;
	vector<vector<int>> edge(V + 1, vector<int>(V + 1, 0x7fffffff));
	vector<int> dist(V + 1, 0x7fffffff);

	if (E == 0) {
		cout << -1;
		return 0;
	}

	for (int i = 0; i < E; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		edge[u][v] = w;
		edge[v][u] = w;
	}
	for (int i = 0; i < V; i++) {
		edge[i + 1][i + 1] = 0;
	}

	int v1, v2;
	cin >> v1 >> v2;

	int dv1, dv2, df;
	dijkstra(dist, edge, 1);
	dv1 = dist[v1];
	dv2 = dist[v2];
	if ((dv1 < 0 || dv1 == INF) && (dv2 < 0 || dv2 == INF)) {
		cout << -1;
		return 0;
	}

	dist = vector<int>(V + 1, 0x7fffffff);
	dijkstra(dist, edge, v1);
	df = dist[v2];

	if (df < 0 || df == INF) {
		cout << -1;
		return 0;
	}

	dist = vector<int>(V + 1, 0x7fffffff);
	dijkstra(dist, edge, V);

	int ans1, ans2;
	ans1 = dv1 + df + dist[v2];
	ans2 = dv2 + df + dist[v1];

	int ans = min(ans1, ans2);
	if ((dist[v1] < 0 || dist[v1] == INF) && (dist[v2] < 0 || dist[v2] == INF)) {
		cout << -1;
		return 0;
	}

	cout << ans;

	return 0;
}
