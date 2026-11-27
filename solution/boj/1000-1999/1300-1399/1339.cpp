// BOJ 1339 - 단어 수학
// 난이도: Gold IV
// 푼 날짜: 2021-02-24
// https://www.acmicpc.net/problem/1339

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int search(char x, vector<char> save) {
	for (unsigned i = 0; i < save.size(); i++) {
		if (save[i] == x) return i;
	}
	return -1;
}

int powof10(int power) {
	if (power) return 10 * powof10(power - 1);
	else return 1;
}

int main(void){
	int n, sum = 0;
	vector<char> save;
	int digit[10] = {};
	cin >> n;

	for (int i = 0; i < n; i++) {
		string input;
		cin >> input;
		for (unsigned j = 0; j < input.size(); j++) {
			int index = search(input[j], save);
			if (index < 0) {
				save.push_back(input[j]);
				digit[save.size() - 1] += powof10((int) input.size() - j - 1);
			}
			else {
				digit[index] += powof10((int) input.size() - j - 1);
			}
		}
	}
	sort(digit, digit + 10, greater<int>());
	for (int i = 0; i < 10; i++) {
		sum += digit[i] * (9 - i);
	}
	cout << sum;
} //
