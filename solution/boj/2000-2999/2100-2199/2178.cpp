// BOJ 2178 - 미로 탐색
// 난이도: Silver I
// 푼 날짜: 2021-06-20
// https://www.acmicpc.net/problem/2178

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>

#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

struct Point {
	int x;
	int y;

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

int main(void) {
	int N, M, level = 0;
	cin >> N >> M;
	queue<Point> q;

	char** maze = new char* [N];
	bool** visited = new bool* [N];

	cin.get();
	for (int i = 0; i < N; i++) {
		maze[i] = new char[M];
		visited[i] = new bool[M];
		
		for (int j = 0; j < M; j++) {
			maze[i][j] = cin.get();
			visited[i][j] = 0;
		}
		cin.get();
	}

	visited[0][0] = 1;
	q.push(Point(0, 0));

	while (!q.empty()) {
		level++;
		unsigned qsize = q.size();
		for (unsigned i = 0; i < qsize; i++) {
			Point p = q.front();
			if ((p.x == N - 1) && (p.y == M - 1)) {
				goto Out;
			}
			if ((p.x != 0) && (!visited[p.x - 1][p.y]) && (maze[p.x - 1][p.y] == '1')) {
				visited[p.x - 1][p.y] = 1;
				q.push(Point(p.x - 1, p.y));
			}
			if ((p.x != N - 1) && (!visited[p.x + 1][p.y]) && (maze[p.x + 1][p.y] == '1')) {
				visited[p.x + 1][p.y] = 1;
				q.push(Point(p.x + 1, p.y));
			}
			if ((p.y != 0) && (!visited[p.x][p.y - 1]) && (maze[p.x][p.y - 1] == '1')) {
				visited[p.x][p.y - 1] = 1;
				q.push(Point(p.x, p.y - 1));
			}
			if ((p.y != M - 1) && (!visited[p.x][p.y + 1]) && (maze[p.x][p.y + 1] == '1')) {
				visited[p.x][p.y + 1] = 1;
				q.push(Point(p.x, p.y + 1));
			}
			q.pop();
		}
	}
Out:
	cout << level;
	for (int i = 0; i < N; i++) {
		delete[] maze[i];
		delete[] visited[i];
	}
	delete[] maze;
	delete[] visited;

	return 0;
}
