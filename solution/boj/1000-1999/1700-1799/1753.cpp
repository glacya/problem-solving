// BOJ 1753 - 최단경로
// 난이도: Gold IV
// 푼 날짜: 2021-06-30
// https://www.acmicpc.net/problem/1753

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>

#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/
#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

struct comp {
	bool operator()(pair<int, int> p1, pair<int, int> p2) {
		return p1.second > p2.second;
	}
};

void dijkstra(vector<int>& dist, vector<unordered_map<int, int>> &edge, int vertex) {
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

	int V, E, start;
	cin >> V >> E >> start;
	vector<unordered_map<int, int>> edge(V + 1);
	vector<int> dist(V + 1, 0x7fffffff);

	for (int i = 0; i < E; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		if ((edge[u].find(v) == edge[u].end()) || (edge[u][v] > w)) {
			edge[u][v] = w;
		}
	}

	dijkstra(dist, edge, start);

	for (int i = 1; i <= V; i++) {
		if (dist[i] == 0x7fffffff) cout << "INF\n";
		else cout << dist[i] << "\n";
	}

	return 0;
}
