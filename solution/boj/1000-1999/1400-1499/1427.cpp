// BOJ 1427 - 소트인사이드
// 난이도: Silver V
// 푼 날짜: 2021-08-05
// https://www.acmicpc.net/problem/1427

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <cstring>
#include <cassert>

#pragma warning(disable : 4996)

#define cheatkey ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

using namespace std;

typedef long long ll;

int main(void) {
	char number[11];
	scanf("%s", number);

	sort(number, number + strlen(number), greater<int>());
	printf("%s", number);

}
