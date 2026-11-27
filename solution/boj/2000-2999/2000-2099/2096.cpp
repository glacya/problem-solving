// BOJ 2096 - 내려가기
// 난이도: Gold V
// 푼 날짜: 2022-04-14
// https://www.acmicpc.net/problem/2096

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

int get_min(vector<int>& v) {
	int m = 0x7fffffff;
	for (int x : v) {
		if (m > x) m = x;
	}
	return m;
}

int get_max(vector<int>& v) {
	int m = 0;
	for (int x : v) {
		if (m < x) m = x;
	}
	return m;
}

int main(void) {
	cheatkey;

	int N;
	cin >> N;

	vector<int> minscore(3, 0);
	vector<int> maxscore(3, 0);

	for (int i = 0; i < N; i++) {
		int b1, b2, b3;
		cin >> b1 >> b2 >> b3;
		int i1, i2, i3, x1, x2, x3;

		i1 = min(minscore[0], minscore[1]) + b1;
		i2 = get_min(minscore) + b2;
		i3 = min(minscore[1], minscore[2]) + b3;

		x1 = max(maxscore[0], maxscore[1]) + b1;
		x2 = get_max(maxscore) + b2;
		x3 = max(maxscore[1], maxscore[2]) + b3;

		minscore[0] = i1;
		minscore[1] = i2;
		minscore[2] = i3;

		maxscore[0] = x1;
		maxscore[1] = x2;
		maxscore[2] = x3;
	}

	cout << get_max(maxscore) << " " << get_min(minscore);

	return 0;
}
