// BOJ 1111 - IQ Test
// 난이도: Gold III
// 푼 날짜: 2021-02-20
// https://www.acmicpc.net/problem/1111

#include <iostream>
#include <vector>

using namespace std;

int main(void) {
	int n, a, b;
	int hang;
	cin >> n;
	
	int* v = new int[n];
	int* diff = new int[n - 1];

	for (int i = 0; i < n; i++) {
		cin >> hang;
		v[i] = hang;
	}

	if (n < 3) {
		if ((n == 2) && !(v[1] - v[0])) {
			cout << v[0];
			delete[] v, diff;
			return 0;
		}
		else {
			cout << "A";
			delete[] v, diff;
			return 0;
		}
	}

	for (int i = 0; i < n - 1; i++) {
		diff[i] = v[i + 1] - v[i];
	}

	if (diff[0]) {
		if (diff[1]) {
			int ratio = diff[1] / diff[0];
			if (diff[1] % diff[0]) {
				cout << "B";
				delete[] v, diff;
				return 0;
			}
			for (int i = 2; i < n - 1; i++) {
				if (!diff[i] || ratio != diff[i] / diff[i - 1] || diff[i] % diff[i-1]) {
					cout << "B";
					delete[] v, diff;
					return 0;
				}
			}
		}
		else {
			for (int i = 2; i < n - 1; i++) {
				if (diff[i]) {
					cout << "B";
					delete[] v, diff;
					return 0;
				}
			}
		}
	}
	else {
		for (int i = 1; i < n - 1; i++) {
			if (diff[i]) {
				cout << "B";
				delete[] v, diff;
				return 0;
			}
		}
		cout << v[0];
		delete[] v, diff;
		return 0;
	}
	a = diff[1] / diff[0];
	b = v[1] - a * v[0];
	cout << a * v[n - 1] + b;
	delete[] v, diff;
	return 0;
}
