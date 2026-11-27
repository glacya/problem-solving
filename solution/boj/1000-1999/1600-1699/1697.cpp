// BOJ 1697 - 숨바꼭질
// 난이도: Silver I
// 푼 날짜: 2021-02-23
// https://www.acmicpc.net/problem/1697

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct vertex {
	int value;
	int level;

	vertex(int v, int l) {
		this->value = v;
		this->level = l;
	}
};

int main(void){
	int start, dest;
	cin >> start >> dest;
	bool* visited = new bool[100001]();
	queue<vertex> queue;

	if (start >= dest) {
		cout << start - dest;
		return 0;
	}
	else {
		queue.push(vertex(start, 0));
		while (1) {
			vertex term = queue.front();
			int c1 = term.value + 1, c2 = term.value - 1, c3 = term.value * 2;
			if (c1 == dest || c2 == dest || c3 == dest) {
				cout << term.level + 1;
				break;
			}
			if ((c1 <= 100000)) {
				if (!visited[c1]) {
					visited[c1] = 1;
					queue.push(vertex(c1, term.level + 1));
				}
			}
			if ((c2 <= 100000 && c2 >= 0)) {
				if (!visited[c2]) {
					visited[c2] = 1;
					queue.push(vertex(c2, term.level + 1));
				}
			}
			if ((c3 <= 100000)) {
				if (!visited[c3]) {
					visited[c3] = 1;
					queue.push(vertex(c3, term.level + 1));
				}
			}
			queue.pop();
		}
	}
	delete[] visited;
	return 0;
} // 1697
