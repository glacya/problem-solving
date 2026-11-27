// BOJ 1916 - 최소비용 구하기
// 난이도: Gold V
// 푼 날짜: 2021-06-29
// https://www.acmicpc.net/problem/1916

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <stack>

#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

struct comp {
	bool operator()(pair<int, int> p1, pair<int, int> p2) {
		return p1.second > p2.second;
	}
};

void dijkstra(vector<int>& dist, vector<vector<int>>& edge, int vertex) {
	priority_queue<pair<int, int>, vector<pair<int, int>>, comp> pq;
	pq.push(make_pair(vertex, 0));
	dist[vertex] = 0;
	
	for (int j = 0; j < edge.size() - 2; j++) {
		int t = pq.top().first;
		pq.pop();

		for (int i = 1; i < (int)edge.size(); i++) {
			if ((edge[t][i] != 0x7fffffff) && (dist[i] > dist[t] + edge[t][i])) {
				dist[i] = dist[t] + edge[t][i];
				pq.push(make_pair(i, dist[i]));
			}
		}
	}

}


int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int V, E;
	cin >> V >> E;
	vector<vector<int>> edge(V + 1, vector<int>(V + 1, 0x7fffffff));
	vector<int> dist(V + 1, 0x7fffffff);

	for (int i = 0; i < E; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		if (edge[u][v] > w) edge[u][v] = w;
		//edge[v][u] = w;
	}

	int v1, v2;
	cin >> v1 >> v2;

	dijkstra(dist, edge, v1);
	cout << dist[v2];

	return 0;
}
