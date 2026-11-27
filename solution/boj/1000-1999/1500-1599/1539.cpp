// BOJ 1539 - 이진 검색 트리
// 난이도: Platinum V
// 푼 날짜: 2021-06-21
// https://www.acmicpc.net/problem/1539

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <map>


#pragma warning(disable : 4996)
/*
ios::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);
*/

using namespace std;

int main(void) {
	map<int, int> M;
	int N;
	long long sum = 0;
	cin >> N;
	for (int i = 0; i < N; i++) {
		int k;
		cin >> k;
		if (!i) {
			M[k] = 1;
			sum++;
		}
		else {
			auto iter = M.lower_bound(k);
			auto iter2 = iter;
			if (iter != M.begin()) iter--;
			int val = (iter->second < iter2->second) ? iter2->second : iter->second;
			M[k] = val + 1;
			sum += (long long)(val + 1);
			/*
			cout << "k = " << k << ", M[" << iter->first << "] = " << iter->second << ", M[" << iter2->first << "] = " << iter2->second << endl;
			for (auto i = M.begin(); i != M.end(); i++) {
				cout << "[" << i->first << " : " << i->second << "]\t";
			}
			cout << endl;
			*/
		}
	}
	cout << sum;
	

	return 0;
}
