// BOJ 1012 - 유기농 배추
// 난이도: Silver II
// 푼 날짜: 2021-02-21
// https://www.acmicpc.net/problem/1012

#include <iostream>
#include <vector>

using namespace std;
vector<int> q;

void dfs(int point, int xsize, int ysize) {
	int neigh[5] = { point, -1, -1, -1, -1 }; // up down left right;

	if (point < 0) return;

	if (point < xsize * (ysize - 1)) neigh[1] = point + xsize;	// up
	if (point >= xsize) neigh[2] = point - xsize;				// down
	if (point % xsize) neigh[3] = point - 1 ;					// left
	if (point % xsize < xsize - 1) neigh[4] = point + 1;		// right
	
	for (int i = 0; i < 5; i++) {
		if (neigh[i] >= 0) {
			for (unsigned j = 0; j < q.size(); j++) {
				if (neigh[i] == q[j]) {
					q[j] = -2;
					if (i) dfs(neigh[i], xsize, ysize);
				}
			}
		}
		else continue;
	}
}

void routine(void) {
	int M, N, k, count = 0; // 가로 M, 세로 N
	cin >> M >> N >> k;
	q = vector<int>();

	for (int i = 0; i < k; i++) {
		int x, y, point;
		cin >> x >> y;

		point = x + y * M;
		q.push_back(point);
	}

	for (int i = 0; i < k; i++) {
		if (q[i] >= 0) {
			dfs(q[i], M, N);
			count++;
		}
		else continue;
	}

	cout << count << endl;
}

int main(void){
	int testcase;
	cin >> testcase;

	for (int t = 0; t < testcase; t++) {
		routine();
	}
}
// 1012
