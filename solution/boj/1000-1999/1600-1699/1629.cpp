// BOJ 1629 - 곱셈
// 난이도: Silver I
// 푼 날짜: 2021-03-01
// https://www.acmicpc.net/problem/1629

#define _CRT_SECURE_NO_WARNINGS // scanf 사용가능

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

/* 마법의 주문
ios_base::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

int main(void) {
	int base, power, mod;
	cin >> base >> power >> mod;
	long long remainder[32];
	long long result = 1;

	remainder[0] = base % mod;
	for (int i = 1; i < 32; i++) {
		remainder[i] = remainder[i - 1] * remainder[i - 1] % mod;
	}

	for (int i = 0; i < 32; i++) {
		if ((1 << i) & power) {
			result = result * remainder[i] % mod;
		}
	}
	cout << result;

	return 0;
}
// 1629
