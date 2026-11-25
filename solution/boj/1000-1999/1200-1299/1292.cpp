// BOJ 1292 - 쉽게 푸는 문제
// 난이도: Bronze I
// 푼 날짜: 2021-06-20
// https://www.acmicpc.net/problem/1292

#include <iostream>
#include <string>
#include <algorithm>

#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

int sum(int n) {
	return n * (n + 1) / 2;
}

int ssqr(int n) {
	return n * (n + 1) * (2 * n + 1) / 6;
}

int main(void) {
	int a, b;
	cin >> a >> b;
	a--;
	int ai = 44, bi = 44;

	int checkpoints[45];
	for (int i = 0; i < 45; i++) {
		checkpoints[i] = (i + 1) * i / 2;
	}
	for (int i = 0; i < 45; i++) {
		if (checkpoints[i] > a) {
			ai = i - 1;
			break;
		}
	}
	for (int i = 0; i < 45; i++) {
		if (checkpoints[i] > b) {
			bi = i - 1;
			break;
		}
	}
	int asum = (a - sum(ai)) * (ai + 1) + ssqr(ai);
	int bsum = (b - sum(bi)) * (bi + 1) + ssqr(bi);

	cout << bsum - asum << endl;

	return 0;
}
