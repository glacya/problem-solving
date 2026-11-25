// BOJ 1149 - RGB거리
// 난이도: Silver I
// 푼 날짜: 2021-07-02
// https://www.acmicpc.net/problem/1149

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <cmath>
#include <string>

#pragma warning(disable : 4996)

#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

typedef long long ll;


int MIN(int a, int b) {
	if (a < b) return a;
	else return b;
}

int main(void) {
	cheatkey;

	int N;
	cin >> N;

	vector<vector<int>> rgb(N);
	vector<vector<int>> dp(N, vector<int>(3, 0));

	for (int i = 0; i < N; i++) {
		int n1, n2, n3;
		cin >> n1 >> n2 >> n3;
		rgb[i] = { n1, n2, n3 };
	}

	dp[0][0] = rgb[0][0];
	dp[0][1] = rgb[0][1];
	dp[0][2] = rgb[0][2];

	for (int i = 1; i < N; i++) {
		dp[i][0] = rgb[i][0] + MIN(dp[i - 1][1], dp[i - 1][2]);
		dp[i][1] = rgb[i][1] + MIN(dp[i - 1][0], dp[i - 1][2]);
		dp[i][2] = rgb[i][2] + MIN(dp[i - 1][0], dp[i - 1][1]);
	}

	cout << MIN(MIN(dp[N - 1][0], dp[N - 1][1]), dp[N - 1][2]);

	return 0;
}
