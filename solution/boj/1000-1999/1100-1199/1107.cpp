// BOJ 1107 - 리모컨
// 난이도: Gold IV
// 푼 날짜: 2021-06-20
// https://www.acmicpc.net/problem/1107

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>

#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

int valid(string &str, vector<char> &a) {
	for (auto x : str) {
		bool no = true;
		for (auto y : a) {
			if (x == y) {
				no = false;
				break;
			}
		}
		if (no) {
			return -1;
		}
	}
	return 0;
}

int main(void) {
	int target;
	int c, min = 500000;
	cin >> target >> c;
	vector<char> allowed = {'0','1','2','3','4','5','6','7','8','9'};

	for (int i = 0; i < c; i++) {
		char k;
		cin >> k;
		auto iter = allowed.begin();
		for (auto x = iter; x != allowed.end(); x++) {
			if (*x == k) {
				allowed.erase(x);
				break;
			}
		}
	}
	
	for (int i = 0; i < 1000000; i++) {
		string k = to_string(i);
		if (valid(k, allowed) < 0) continue;
		int a = abs(target - i) + k.size();
		if (min > a) {
			min = a;
		}
	}
	int b = abs(target - 100);
	if (min > b) {
		min = b;
	}
	cout << min;

	return 0;
}
