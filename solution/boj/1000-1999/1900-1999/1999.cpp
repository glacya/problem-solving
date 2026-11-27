// BOJ 1999 - 최대최소
// 난이도: Gold IV
// 푼 날짜: 2023-10-11
// https://www.acmicpc.net/problem/1999

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, b, q;
    cin >> n >> b >> q;
    
    int c = n - b + 1;
    vector<vector<int>> mat(n, vector<int>(n));
    vector<vector<int>> minc(n, vector<int>(c));
    vector<vector<int>> maxc(n, vector<int>(c));
    vector<vector<int>> minf(c, vector<int>(c));
    vector<vector<int>> maxf(c, vector<int>(c));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> mat[i][j];
        }
    }
    
    // Compress by y-axis
    for (int i = 0; i < n; i++) {
        priority_queue<pair<int, int>> pq_max;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq_min;
        
        for (int j = 0; j < n; j++) {
            pair<int, int> entry(mat[i][j], j);
            pq_max.push(entry);
            pq_min.push(entry);
            
            while (pq_max.top().second < j - b + 1) {
                pq_max.pop();
            }
            while (pq_min.top().second < j - b + 1) {
                pq_min.pop();
            }
            
            if (j - b + 1 >= 0) {
                minc[i][j - b + 1] = pq_min.top().first;
                maxc[i][j - b + 1] = pq_max.top().first;
            }
        }
    }
    
    // Compress by x-axis
    for (int i = 0; i < c; i++) {
        priority_queue<pair<int, int>> pq_max;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq_min;
        
        for (int j = 0; j < n; j++) {
            pair<int, int> min_entry(minc[j][i], j);
            pair<int, int> max_entry(maxc[j][i], j);
            pq_min.push(min_entry);
            pq_max.push(max_entry);
            
            while (pq_max.top().second < j - b + 1) {
                pq_max.pop();
            }
            while (pq_min.top().second < j - b + 1) {
                pq_min.pop();
            }
            
            if (j - b + 1 >= 0) {
                minf[j - b + 1][i] = pq_min.top().first;
                maxf[j - b + 1][i] = pq_max.top().first;
            }
        }
    }
    
    while (q--) {
        int x, y;
        cin >> x >> y;
        
        cout << maxf[x - 1][y - 1] - minf[x - 1][y - 1] << "\n";
    }
}
