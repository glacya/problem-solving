// BOJ 1865 - 웜홀
// 난이도: Gold III
// 푼 날짜: 2022-02-19
// https://www.acmicpc.net/problem/1865

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

struct hashfunc {
	int operator()(const pair<int, int> input) const {
		return input.first * 100 + input.second;
	}
}; 

int main(void) {
	cheatkey;

	int tc;
	cin >> tc;

	for (int quar = 0; quar < tc; quar++) {
		int node, road, wormhole;
		cin >> node >> road >> wormhole;

		unordered_map<pair<int, int>, int, hashfunc> edges;
		vector<int> dist = vector<int>(node + 1, 999999999);
		dist[1] = 0;
		
		for (int i = 0; i < road; i++) {
			int start, end, time;
			cin >> start >> end >> time;
			pair<int, int> e1 = make_pair(start, end);
			pair<int, int> e2 = make_pair(end, start);

			if (edges.find(e1) == edges.end() || edges[e1] > time) {
				edges[e1] = time;
			}
			if (edges.find(e2) == edges.end() || edges[e2] > time) {
				edges[e2] = time;
			}
		}
		
		for (int i = 0; i < wormhole; i++) {
			int start, end, time;
			cin >> start >> end >> time;

			pair<int, int> e = make_pair(start, end);
			if (edges.find(e) == edges.end() || edges[e] > -time) {
				edges[e] = -time;
			}
		}

		for (int i = 0; i < node - 1; i++) {
			for (auto &edge : edges) {
				auto pair = edge.first;
				if (dist[pair.first] + edge.second < dist[pair.second]) {
					dist[pair.second] = dist[pair.first] + edge.second;
				}
			}
		}
		bool reversal = true;
		for (auto &edge : edges) {
			auto pair = edge.first;
			if (dist[pair.first] + edge.second < dist[pair.second]) {
				cout << "YES\n";
				reversal = false;
				break;
			}
		}
		if (reversal) {
			cout << "NO\n";
		}
	}
	
}
