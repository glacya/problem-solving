// BOJ 1297 - TV 크기
// 난이도: Bronze II
// 푼 날짜: 2021-02-24
// https://www.acmicpc.net/problem/1297

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

int main(void) {
	double c, a, b;
	cin >> c >> a >> b;

	double l = sqrt(c*c/(a*a + b*b));
	cout << (int) (a * l) << " " << (int) (b * l);
} //
