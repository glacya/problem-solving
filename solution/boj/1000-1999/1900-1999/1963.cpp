// BOJ 1963 - 소수 경로
// 난이도: Gold IV
// 푼 날짜: 2021-02-20
// https://www.acmicpc.net/problem/1963

#include <iostream>
#include <vector>
using namespace std;

int search(int target, int p[]) {
	for (int i = 168; i < 1229; i++) {
		if (target == p[i]) return 1;
	}
	return 0;
}

vector<int> makelist(int target, int p[]) {
	vector<int> val;
	int vx = 0;
	int num1 = target % 10; // 일의자리
	int num2 = (target / 10) % 10;  // 십의자리
	int num3 = (target / 100) % 10; // 백의자리
	int num4 = target / 1000; // 천의자리

	for (int i = 1; i < 10; i += 2) {
		if (i == 5) continue;
		int cal = target + i - num1;
		if (cal == target) continue;
		if (search(cal, p)) val.push_back(cal);
	}

	for (int i = 0; i < 10; i++) {
		int cal = target + 10 * (i - num2);
		if (cal == target) continue;
		if (search(cal, p)) val.push_back(cal);
	}

	for (int i = 0; i < 10; i++) {
		int cal = target + 100 * (i - num3);
		if (cal == target) continue;
		if (search(cal, p)) val.push_back(cal);
	}

	for (int i = 1; i < 10; i++) {
		int cal = target + 1000 * (i - num4);
		if (cal == target) continue;
		if (search(cal, p)) val.push_back(cal);
	}
	return val;
}

int routine(int start, int end, int* p) {
	int level = 1;
	vector<int> queue; // 처리해야 할 정점
	vector<int> visited; // 처리된 정점

	if (start == end) {
		cout << 0 << endl;
		return 0;
	}

	queue.push_back(start);
	
	while (1) {  // level 단위
		int vsize = visited.size();
		int qsize = queue.size();
		for (int i = 0; i < qsize; i++) {  // queue에 든 원소 처리
			visited.push_back(queue[i]);
		}
		queue.clear();
		for (int i = vsize; i < vsize + qsize; i++) {
			vector<int> ret = makelist(visited[i], p);
			for (unsigned j = 0; j < ret.size(); j++) { // ret의 원소 순회
				bool pass = true;
				for (unsigned k = 0; k < visited.size(); k++) { // visited의 원소 순회
					if (visited[k] == ret[j]) {
						pass = false;
						break;
					}
					if (ret[j] == end) {
						cout << level << endl;
						return 0;
					}
				}
				for (unsigned k = 0; k < queue.size(); k++) {
					if (queue[k] == ret[j]) {
						pass = false;
						break;
					}
				}
				if (pass) {
					queue.push_back(ret[j]);
				}
			}
		}
		if (queue.empty()) {
			cout << "Impossible" << endl;
			return 0;
		}
		level++;
	}
	return 0;
}

int main(void) {
	int* p = new int[1229]; // 168번 index부터 출발, 총 1061개의 소수
	int c = 5, level = 2;
	int n;
	vector<int> start, end;
	int s, e;

	p[0] = 2;
	p[1] = 3;

	while (c <= 9999) {
		bool add = true;
		for (int i = 0; i < level; i++) {
			if (c % p[i] == 0) {
				add = false;
				break;
			}
			if (c < p[i] * p[i]) break;
		}
		if (add) {
			p[level++] = c;
		}
		c += 2;
	}

	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> s >> e;
		start.push_back(s);
		end.push_back(e);
	}
	for (int i = 0; i < n; i++) {
		routine(start[i], end[i], p);
	}
	delete[] p;
	return 0;
}
