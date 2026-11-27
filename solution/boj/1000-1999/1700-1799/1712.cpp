// BOJ 1712 - 손익분기점
// 난이도: Bronze II
// 푼 날짜: 2021-02-21
// https://www.acmicpc.net/problem/1712

#include <iostream>

using namespace std;

int main(void) {
	int a, b, c;
	cin >> a >> b >> c;

	if (c - b > 0) {
		cout << a / (c - b) + 1;
	}
	else cout << -1;

	return 0;
}
