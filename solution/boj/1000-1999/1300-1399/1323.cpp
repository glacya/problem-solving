// BOJ 1323 - 숫자 연결하기
// 난이도: Gold IV
// 푼 날짜: 2021-02-22
// https://www.acmicpc.net/problem/1323

#include <iostream>
#include <vector>

using namespace std;

int main(void) {
	long long n, k;
	cin >> n >> k;
	int count = 0;

	long long nmod = n % k;
	long long dmod = 1;
	long long sum = nmod;

	for (long long i = n; i >= 10; i = i / 10) {
		dmod *= 10;
	}
	dmod = dmod * 10 % k;
	for (unsigned i = 0; i < k; i++) {
		if (sum == 0) {
			cout << count + 1;
			return 0;
		}
		sum = (sum * dmod + nmod) % k;
		count++;
	}

	cout << -1;
	return 0;
}
