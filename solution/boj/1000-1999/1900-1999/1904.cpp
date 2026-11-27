// BOJ 1904 - 01타일
// 난이도: Silver III
// 푼 날짜: 2022-02-17
// https://www.acmicpc.net/problem/1904

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

	int* dp = new int[1000001];
	int N;
	cin >> N;

	dp[0] = 1;
	dp[1] = 1;
	for (int i = 2; i <= N; i++) {
		dp[i] = (dp[i - 1] + dp[i - 2]) % 15746;
	}
	cout << dp[N] % 15746;

	delete[] dp;
}
