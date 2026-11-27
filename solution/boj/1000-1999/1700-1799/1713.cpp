// BOJ 1713 - 후보 추천하기
// 난이도: Silver I
// 푼 날짜: 2021-08-12
// https://www.acmicpc.net/problem/1713

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
	cheatkey;

	int N, chu;
	cin >> N >> chu;

	vector<pair<int, int>> stdnum;

	for (int i = 0; i < chu; i++) {
		int vote;
		cin >> vote;

		bool exists = false;
		for (int j = 0; j < (int)stdnum.size(); j++) {
			if (vote == stdnum[j].first) {
				exists = true;
				stdnum[j].second++;
				break;
			}
		}
		if (!exists) {
			if ((int)stdnum.size() >= N) {
				auto miniter = stdnum.begin();
				for (auto iter = stdnum.begin(); iter != stdnum.end(); iter++) {
					if (iter->second < miniter->second) {
						miniter = iter;
					}
				}
				stdnum.erase(miniter);
			}
			stdnum.push_back(make_pair(vote, 1));
		}
	}

	sort(stdnum.begin(), stdnum.end());

	for (auto x : stdnum) {
		cout << x.first << " ";
	}
}
