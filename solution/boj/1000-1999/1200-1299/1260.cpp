// BOJ 1260 - DFS와 BFS
// 난이도: Silver II
// 푼 날짜: 2021-02-26
// https://www.acmicpc.net/problem/1260

#define _CRT_SECURE_NO_WARNINGS // scanf 사용가능

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

/* 마법의 주문
ios_base::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

void DFS(int vertex, bool* dpsv, const vector<vector<int>>& edge) {
	cout << vertex << " ";
	for (unsigned i = 0; i < edge[vertex].size(); i++) {
		int neighbor = edge[vertex][i];
		if (!dpsv[neighbor]) {
			dpsv[neighbor] = 1;
			DFS(neighbor, dpsv, edge);
		}
	}
}

int main(void) {
	int v, edgenum, startvertex;
	cin >> v >> edgenum >> startvertex;

	bool* dpsvisited = new bool[v + 1]();
	bool* bpsvisited = new bool[v + 1]();
	vector<vector<int>> edge(v + 1, vector<int>());
	queue<int> q;

	for (int i = 0; i < edgenum; i++) {
		int v1, v2;
		cin >> v1 >> v2;
		edge[v1].push_back(v2);
		edge[v2].push_back(v1);
	}

	for (int i = 1; i <= v; i++) {
		sort(edge[i].begin(), edge[i].end());
	}

	// DFS 부분
	dpsvisited[startvertex] = 1;
	DFS(startvertex, dpsvisited, edge);


	//

	cout << "\n";

	// BPS 부분

	q.push(startvertex);
	bpsvisited[startvertex] = 1;
	while (q.size()) {
		int target = q.front();
		cout << target << " ";
		for (unsigned i = 0; i < edge[target].size(); i++) {
			int neighbor = edge[target][i];
			if (!bpsvisited[neighbor]) {
				bpsvisited[neighbor] = 1;
				q.push(neighbor);
			}
		}
		q.pop();
	}

	// 

	delete[] dpsvisited;
	delete[] bpsvisited;

	return 0;
} // 1260
