// BOJ 2263 - 트리의 순회
// 난이도: Gold I
// 푼 날짜: 2021-06-22
// https://www.acmicpc.net/problem/2263

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

void preorder(vector<int>& inorder, vector<int>& postorder, int left, int right, int offset) {
	//printf("\npreorder(%d, %d, %d) ", left, right, offset);
	
	int root = postorder[right - 1];

	cout << root << " ";
	if (right - left == 1) return;
	if (right - left == 2) {
		cout << postorder[left] << " ";
		return;
	}
	int index = inorder[root] - offset;

	if (index > left) preorder(inorder, postorder, left, index, offset);
	if (right - 1 > index) preorder(inorder, postorder, index, right - 1, offset + 1);
}

int main(void) {
	int N;
	cin >> N;
	vector<int> inorder(N + 1);
	vector<int> postorder;

	for (int i = 0; i < N; i++) {
		int num;
		cin >> num;
		inorder[num] = i;
	}
	for (int i = 0; i < N; i++) {
		int num;
		cin >> num;
		postorder.push_back(num);
	}

	preorder(inorder, postorder, 0, N, 0);

	return 0;
}
