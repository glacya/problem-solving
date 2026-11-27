// BOJ 1463 - 1로 만들기
// 난이도: Silver III
// 푼 날짜: 2021-02-21
// https://www.acmicpc.net/problem/1463

#include <iostream>
#include <vector>

using namespace std;

int check(int qx, int target, int* q) {
	for (int i = 0; i < qx; i++) {
		if (q[i] == target) {
			return 0;
		}
	}
	return 1;
}

int main(void){
	int n, qx = 1;
	cin >> n;
	int* visited = new int[n + 1]; // i번째 원소는 i번째 원소로 가기 위한 최단거리
	int* queue = new int[n];

	queue[0] = n;
	visited[n] = 0;

	if (n == 1) {
		cout << 0;
		return 0;
	}

	for (int i = 0; i < n; i++) {
		if (queue[i] % 3 == 0) {
			int c1 = queue[i] / 3;
			if (check(qx, c1, queue)) {
				visited[c1] = visited[queue[i]] + 1;
				if (c1 == 1) break;
				queue[qx++] = c1;
			}
		}
		if (queue[i] % 2 == 0) {
			int c2 = queue[i] / 2;
			if (check(qx, c2, queue)) {
				visited[c2] = visited[queue[i]] + 1;
				if (c2 == 1) break;
				queue[qx++] = c2;
			}
		}
		int c3 = queue[i] - 1;
		if (check(qx, c3, queue)) {
			visited[c3] = visited[queue[i]] + 1;
			if (c3 == 1) break;
			queue[qx++] = c3;
		}
	}
	cout << visited[1];

	delete[] visited;
	delete[] queue;
	return 0;
}
// 1463
