// BOJ 1991 - 트리 순회
// 난이도: Silver I
// 푼 날짜: 2021-06-22
// https://www.acmicpc.net/problem/1991

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

void printout(map<char, pair<char, char>>& M, char c ,int order) {
	if (c == '.') return;
	auto p = M[c];
	switch (order) {
	case (0): {	// preorder : root -> left -> right
		cout << c;
		printout(M, p.first, order);
		printout(M, p.second, order);
		break;
	}
	case (1): {	// inorder : left -> root -> right
		printout(M, p.first, order);
		cout << c;
		printout(M, p.second, order);
		break;
	}
	case (2): {	// postorder : left -> right -> root
		printout(M, p.first, order);
		printout(M, p.second, order);
		cout << c;
		break;
	}
	}
	return;
}

int main(void) {
	int N;
	cin >> N;
	map<char, pair<char, char>> M;

	for (int i = 0; i < N; i++) {
		char node, left, right;
		cin >> node >> left >> right;
		M[node] = make_pair(left, right);
	}
	for (int i = 0; i < 3; i++) {
		printout(M, 'A', i);
		cout << endl;
	}

	return 0;
}
