// BOJ 1500 - 최대 곱
// 난이도: Silver II
// 푼 날짜: 2021-02-23
// https://www.acmicpc.net/problem/1500

#include <iostream>
#include <vector>

using namespace std;

int main(void) {
	long long n, k;
	long long mul = 1;
	long long quot, rem;

	cin >> n >> k;
	quot = n / k;
	rem = n % k;

	for (long long i = 0; i < k; i++) {
		if (rem) {
			mul *= quot + 1;
			rem--;
		}
		else {
			mul *= quot;
		}
	}

	cout << mul;
}
