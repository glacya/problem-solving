// BOJ 1699 - 제곱수의 합
// 난이도: Silver II
// 푼 날짜: 2021-07-09
// https://www.acmicpc.net/problem/1699

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <cmath>
#include <string>
#include <cassert>

#pragma warning(disable : 4996)

#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

typedef long long ll;

int MAX(int a, int b) {
	if (a < b) return b;
	else return a;
}

int main(void) {
	//cheatkey;

	int N;
	cin >> N;

	vector<int> squares;
	vector<int> dp = { 0, 1};


	for (int i = 2; i <= N; i++) {
		//printf("i = %d\n", i);
		int mincount = 1000000;
		for (int j = 1; j * j <= i; j++) {
			//printf("\tExamining dp[%d] = %d and %d\n", i - j * j, dp[i - j * j], j * j);
			if (dp[i - j * j] + 1 < mincount) mincount = dp[i - j * j] + 1;
		}
		//printf("->dp[%d] = %d\n", i, mincount);
		dp.push_back(mincount);
	}

	cout << dp[N];

	return 0;
}
