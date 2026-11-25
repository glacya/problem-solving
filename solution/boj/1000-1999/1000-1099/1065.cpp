// BOJ 1065 - 한수
// 난이도: Silver IV
// 푼 날짜: 2021-02-21
// https://www.acmicpc.net/problem/1065

#include <iostream>
#include <vector>

using namespace std;

int main(void){
	int n, count = 0;
	cin >> n;

	for (int i = 1; i <= n; i++) {
		if (i <= 99) count++;
		else {
			int d1, d2, d3;
			d1 = i / 100;
			d2 = (i / 10) % 10;
			d3 = i % 10;
			if (d2 - d1 == d3 - d2) count++;
		}
	}
	cout << count;
}
// 1065
