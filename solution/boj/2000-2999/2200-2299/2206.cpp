// BOJ 2206 - 벽 부수고 이동하기
// 난이도: Gold III
// 푼 날짜: 2021-07-02
// https://www.acmicpc.net/problem/2206

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <cmath>
#include <string>

#pragma warning(disable : 4996)

#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

typedef long long ll;


int main(void) {
	cheatkey;

	int N, M;
	cin >> N >> M;
	vector<vector<char>> space(N, vector<char>(M, 0));
	vector<vector<vector<bool>>> visited(N, vector<vector<bool>>(M, vector<bool>(2, 0)));

	cin.get();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			space[i][j] = cin.get();
		}
		cin.get();
	}

	queue<tuple<int, int, bool>> q;
	int dist = 0;
	bool reachable = 0;

	visited[0][0] = { 1, 1 };
	q.push(make_tuple(0, 0, 0));

	while (!q.empty()) {
		int qsize = (int)q.size();
		dist++;

		for (int i = 0; i < qsize; i++) {
			const auto &f = q.front();
			int x, y;
			bool passed;
			tie(x, y, passed) = f;
			q.pop();

			if ((x == N - 1) && (y == M - 1)) {
				reachable = 1;
				goto Out;
			}

			if (x != 0) {
				if ((space[x - 1][y] == '1') && !passed && !visited[x - 1][y][1]) {
					visited[x - 1][y][1] = 1;
					q.push(make_tuple(x - 1, y, 1));
				}
				else if ((space[x - 1][y] == '0') && !visited[x - 1][y][passed]) {
					visited[x - 1][y][passed] = 1;
					q.push(make_tuple(x - 1, y, passed));
				}
			}

			if (x != N - 1) {
				if ((space[x + 1][y] == '1') && !passed && !visited[x + 1][y][1]) {
					visited[x + 1][y][1] = 1;
					q.push(make_tuple(x + 1, y, 1));
				}
				else if ((space[x + 1][y] == '0') && !visited[x + 1][y][passed]) {
					visited[x + 1][y][passed] = 1;
					q.push(make_tuple(x + 1, y, passed));
				}
			}

			if (y != 0)  {
				if ((space[x][y - 1] == '1') && !passed && !visited[x][y - 1][1]) {
					visited[x][y - 1][1] = 1;
					q.push(make_tuple(x, y - 1, 1));
				}
				else if ((space[x][y - 1] == '0') && !visited[x][y - 1][passed]) {
					visited[x][y - 1][passed] = 1;
					q.push(make_tuple(x, y - 1, passed));
				}
			}

			if (y != M - 1) {
				if ((space[x][y + 1] == '1') && !passed && !visited[x][y + 1][1]) {
					visited[x][y + 1][1] = 1;
					q.push(make_tuple(x, y + 1, 1));
				}
				else if ((space[x][y + 1] == '0') && !visited[x][y + 1][passed]) {
					visited[x][y + 1][passed] = 1;
					q.push(make_tuple(x, y + 1, passed));
				}
			}
		}

	}
	Out:
	if (reachable) cout << dist;
	else cout << -1;

	return 0;
}
