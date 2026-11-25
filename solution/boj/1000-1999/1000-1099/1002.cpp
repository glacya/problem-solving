// BOJ 1002 - 터렛
// 난이도: Silver III
// 푼 날짜: 2022-02-15
// https://www.acmicpc.net/problem/1002

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>
#include <cmath>

#pragma warning(disable : 4996)

#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

typedef long long ll;

int sq(int x) {
	return x * x;
}

int main(void) {
	cheatkey;

	int T;
	cin >> T;
	for (int i = 0; i < T; i++) {
		int x1, y1, x2, y2, r1, r2;
		cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2;

		int cd = sq(x2 - x1) + sq(y2 - y1);
		int rs = sq(r1 + r2);
		int rd = sq(r1 - r2);

		if (cd == 0 && r1 == r2) cout << "-1\n";
		else if (cd == rs || cd == rd) cout << "1\n";
		else if (cd > rs || cd < rd) cout << "0\n";
		else cout << "2\n";
	}
}
