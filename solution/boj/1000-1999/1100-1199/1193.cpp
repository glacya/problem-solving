// BOJ 1193 - 분수찾기
// 난이도: Silver V
// 푼 날짜: 2021-02-23
// https://www.acmicpc.net/problem/1193

#include <iostream>
#include <vector>

using namespace std;

int main(void) {
	int n, i = 1;
	cin >> n;
	int order;

	while (1) {
		if (n <= i * (i + 1) / 2) {
			break;
		}
		i++;
	}
	
	order = n - i * (i - 1) / 2;
	if (i % 2) {  // 홀수
		cout << i + 1 - order << "/" << order;
	}
	else {            // 짝수
		cout << order << "/" << i + 1 - order;
	}

	return 0;
}
