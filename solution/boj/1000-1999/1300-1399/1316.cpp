// BOJ 1316 - 그룹 단어 체커
// 난이도: Silver V
// 푼 날짜: 2021-08-05
// https://www.acmicpc.net/problem/1316

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>
#include <cassert>

#pragma warning(disable : 4996)

#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

typedef long long ll;

int main(void) {
	int N;
	cin >> N;
	string str;
	int ans = 0;

	for (int i = 0; i < N; i++) {
		cin >> str;
		unordered_set<char> set;
		bool ok = true;
		for (int j = 0; j < (int) str.size(); j++) {
			if (j == 0) {
				set.insert(str[j]);
			}
			else {
				if (str[j - 1] == str[j]) continue;
				else if (set.find(str[j]) == set.end()) {
					set.insert(str[j]);
				}
				else {
					ok = false;
					break;
				}
			}
		}
		if (ok) ans++;
	}
	cout << ans;

	return 0;
}
