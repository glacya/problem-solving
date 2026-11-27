// BOJ 1806 - 부분합
// 난이도: Gold IV
// 푼 날짜: 2021-06-28
// https://www.acmicpc.net/problem/1806

#include <iostream>
#include <vector>
#include <algorithm>

#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int N, S;
	cin >> N >> S;

	vector<int> nums(N);

	int sum = 0;
	int len = N + 1;
	int start = 0, end = 0;

	for (int i = 0; i < N; i++) cin >> nums[i];

	while ((end <= N) && (start < N)) {
		if (sum < S) {
			sum += nums[end];
			end++;
		}
		else {
			if (len > end - start) {
				len = end - start;
			}
			sum -= nums[start];
			start++;
		}
		//printf("start = %d, end = %d, sum = %d\n", start, end, sum);
	}
	if (len > N) {
		if (sum < S) cout << 0;
		else cout << N;
	}
	else cout << len;


	return 0;
}
