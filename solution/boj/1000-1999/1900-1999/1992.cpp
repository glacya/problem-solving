// BOJ 1992 - 쿼드트리
// 난이도: Silver I
// 푼 날짜: 2021-02-26
// https://www.acmicpc.net/problem/1992

#define _CRT_SECURE_NO_WARNINGS // scanf 사용가능

#include <iostream>
#include <vector>
#include <string>
#include <queue>

/* 마법의 주문
ios_base::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

string quadtree(int row, int col, int size, char** pixel) {
	if (size == 1) return string(1, pixel[row][col]);
	else {
		string s[4];
		s[0] = quadtree(row, col, size / 2, pixel);
		s[1] = quadtree(row, col + size / 2, size / 2, pixel);
		s[2] = quadtree(row + size / 2, col, size / 2, pixel);
		s[3] = quadtree(row + size / 2, col + size / 2, size / 2, pixel);
		if (s[0].size() == 1) {
			for (int i = 1; i < 4; i++) {
				if (s[i] != s[0]) {
					return "(" + s[0] + s[1] + s[2] + s[3] + ")";
				}
			}
			return s[0];
		}
		else {
			return "(" + s[0] + s[1] + s[2] + s[3] + ")";
		}
	}
}

int main(void) {
	int n;
	cin >> n;
	char** pixel = new char* [n];

	for (int i = 0; i < n; i++) {
		string input;
		cin >> input;
		pixel[i] = new char[n];

		for (int j = 0; j < n; j++) {
			pixel[i][j] = input[j];
		}
	}

	cout << quadtree(0, 0, n, pixel);

	for (int i = 0; i < n; i++) {
		delete[] pixel[i];
	}

	delete[] pixel;
	return 0;
} // 1992
