// BOJ 1389 - 케빈 베이컨의 6단계 법칙
// 난이도: Silver I
// 푼 날짜: 2021-02-21
// https://www.acmicpc.net/problem/1389

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int search(int target, vector<int> p) { // vector p에 target이 있으면 0, 없으면 1
	for (unsigned i = 0; i < p.size(); i++) {
		if (p[i] == target) return 0;
	}
	return 1;
}

int main(void) {
	int n, m;
	cin >> n >> m;
	int* kevin = new int[n];
	vector<int> rel;

	for (int i = 0; i < m; i++) {
		int p1, p2;
		cin >> p1 >> p2;
		p1--;
		p2--;

		if (search(p1 * n + p2, rel)) rel.push_back(p1 * n + p2);
		if (search(p2 * n + p1, rel)) rel.push_back(p2 * n + p1);
	}

	for (int i = 0; i < n; i++) {
		kevin[i] = 0;
	}

	for (int i = 0; i < n; i++) {
		queue<int> queue;
		vector<int> visited;
		int level = 1;

		visited.push_back(i);
		queue.push(i);
		while (1) {
			int qsize = queue.size();
			if (qsize) {
				for (int j = 0; j < qsize; j++) {
					int rem = queue.front();
					queue.pop();
					for (int k = 0; k < n; k++) {  // rem 번째 사람이 k 번째 사람과 친한가?
						if (rem == k) continue;
						else if (search(k, visited) && !search(k * n + rem, rel)) {
							queue.push(k);
							visited.push_back(k);
							kevin[i] += level;
						}
					}
				}
				
				level++;
			}
			else break;
		}
	}

	int mindex = 0;
	for (int i = 0; i < n; i++) {
		if (kevin[i] < kevin[mindex]) mindex = i;
	}
	cout << mindex + 1;
	delete[] kevin;
	return 0;
}
// 1389
