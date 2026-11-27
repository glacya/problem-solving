// BOJ 1448 - 삼각형 만들기
// 난이도: Silver III
// 푼 날짜: 2023-06-14
// https://www.acmicpc.net/problem/1448

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int n;
    cin >> n;
    
    vector<int> edges(n);
    
    for (int i = 0; i < n; i++) {
        cin >> edges[i];
    }
    
    sort(edges.begin(), edges.end(), std::greater<int>());
    
    int ans = -1;
    
    for (int i = 0; i < n - 2; i++) {
        if (edges[i] < edges[i + 1] + edges[i + 2]) {
            ans = edges[i] + edges[i + 1] + edges[i + 2];
            break;
        }
    }
    cout << ans;

    return 0;
}
