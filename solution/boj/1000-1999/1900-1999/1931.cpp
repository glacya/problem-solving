// BOJ 1931 - 회의실 배정
// 난이도: Gold V
// 푼 날짜: 2021-02-23
// https://www.acmicpc.net/problem/1931

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct offline {
	int start;
	int end;

	offline(int start, int end) {
		this->start = start;
		this->end = end;
	}
};

bool hahaha(offline a, offline b) {
	if (a.start == b.start) return a.end < b.end;
	else return a.start < b.start;
}

int main(void) {
	int n, count = 0, index = 0;
	cin >> n;
	vector<offline> meet;

	for (int i = 0; i < n; i++) {
		int s, e;
		cin >> s >> e;
		meet.push_back(offline(s, e));
	}
	sort(meet.begin(), meet.end(), hahaha);

	while (index < (int) meet.size()) {
		int trav = index;
		int minend = index;
		int endlimit = meet[index].end;
		while (trav < (int)meet.size() && meet[trav].start < endlimit) {
			if (meet[trav].end < meet[minend].end) minend = trav;
			trav++;
		}
		if (meet[minend].end == ~(1 << 31)) {
			index++;
			continue;
		}
		count++;
		while (index < (int)meet.size() && meet[index].start < meet[minend].end) {
			index++;
		}
		meet[minend].end = ~(1 << 31);
	}
	cout << count;

} // 1931
