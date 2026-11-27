// BOJ 1932 - 정수 삼각형
// 난이도: Silver I
// 푼 날짜: 2021-06-22
// https://www.acmicpc.net/problem/1932

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <queue>

#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/


using namespace std;

typedef long long ll;

int max(int a, int b) {
	if (a < b) return b;
	else return a;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int N;
	vector<vector<int>> triangle, dp;
	cin >> N;

	for (int i = 0; i < N; i++) {
		triangle.push_back(vector<int>());
		dp.push_back(vector<int>(i + 1, 0));
		for (int j = 0; j < i + 1; j++) {
			int k;
			cin >> k;
			triangle[i].push_back(k);
		}
	}

	dp[0][0] = triangle[0][0];
	for (int i = 1; i < N; i++) {
		
		for (int j = 0; j < i + 1; j++) {
			if (j == 0) dp[i][j] = dp[i - 1][j] + triangle[i][j];
			else if (j == i) dp[i][j] = dp[i - 1][j - 1] + triangle[i][j];
			else dp[i][j] = max(dp[i - 1][j - 1], dp[i - 1][j]) + triangle[i][j];
		}
	}

	int max = 0;
	for (auto x : dp[N - 1]) {
		if (x > max) max = x;
	}
	cout << max;

	return 0;
}
