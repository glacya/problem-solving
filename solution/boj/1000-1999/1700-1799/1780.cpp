// BOJ 1780 - 종이의 개수
// 난이도: Silver II
// 푼 날짜: 2021-02-26
// https://www.acmicpc.net/problem/1780

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

struct value {
	int minus;
	int zero;
	int plus;

	value(int m, int z, int p) {
		this->minus = m;
		this->zero = z;
		this->plus = p;
	}

	value () {
		this->minus = 0;
		this->zero = 0;
		this->plus = 0;
	}

	value operator +(value v) {
		return value(this->minus + v.minus, this->zero + v.zero, this->plus + v.plus);
	}

	value operator +=(value v) {
		return value(this->minus + v.minus, this->zero + v.zero, this->plus + v.plus);
	}

	bool operator ==(value v) {
		return this->minus == v.minus && this->zero == v.zero && this->plus == v.plus;
	}

	int size(void) {
		return this->minus + this->zero + this->plus;
	}
};

value recval(int row, int col, int size, int** paper) {
	if (size == 1) {
		int a = paper[row][col];
		if (a == 1) return value(0, 0, 1);
		else if (a == 0) return value(0, 1, 0);
		else return value(1, 0, 0);
	}
	else {
		bool unity = true;
		value v[9];
		value sum;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				v[3 * i + j] = recval(row + i * size / 3, col + j * size / 3, size / 3, paper);
				if (i + j == 0) sum = v[0];
				else {
					sum = sum + v[3 * i + j];
					if (!(v[3 * i + j] == v[0])) unity = false;
				}
			}
		}
		if (unity && v[0].size() == 1) return v[0];
		else return sum;
	}
}

int main(void) {
	int papersize;
	cin >> papersize;

	int** paper = new int* [papersize];
	for (int i = 0; i < papersize; i++) {
		paper[i] = new int[papersize];
		for (int j = 0; j < papersize; j++) {
			cin >> paper[i][j];
		}
	}

	value result = recval(0, 0, papersize, paper);

	cout << result.minus << "\n" << result.zero << "\n" << result.plus;

	for (int i = 0; i < papersize; i++) {
		delete[] paper[i];
	}

	delete[] paper;
	return 0;
} // 1780
