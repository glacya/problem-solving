// BOJ 1655 - 가운데를 말해요
// 난이도: Gold II
// 푼 날짜: 2022-03-30
// https://www.acmicpc.net/problem/1655

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

	// the largest one is at the top; should contain small numbers
	std::priority_queue<int> small_pq;
	
	// the smallest one is at the top; should contain large numbers
	std::priority_queue<int, std::vector<int>, std::greater<int>> large_pq;

	int N;
	cin >> N;

	for (int i = 0; i < N; i++) {
		int x;
		cin >> x;

		if (i % 2) {
			large_pq.push(x);
		}
		else {
			small_pq.push(x);
		}

		if (!large_pq.empty() && !small_pq.empty()) {
			int large = small_pq.top();
			int small = large_pq.top();
			if (large > small) {
				small_pq.pop();
				large_pq.pop();
				small_pq.push(small);
				large_pq.push(large);
			}
		}
		
		std::cout << small_pq.top() << "\n";
		
	}

	
	
}
