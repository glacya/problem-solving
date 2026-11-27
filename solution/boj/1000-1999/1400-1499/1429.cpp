// BOJ 1429 - 결혼
// 난이도: Gold II
// 푼 날짜: 2023-10-10
// https://www.acmicpc.net/problem/1429

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

int main() {
    fastio;
    
    int n, m;
    cin >> n >> m;
    
    vector<int> male_bm(n);
    vector<int> female_bm(m);
    
    int male_max = (1 << n) - 1;
    int female_max = (1 << m) - 1;
    
    int impos_male = 0;
    int impos_female = 0;
    
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        
        for (int j = 0; j < m; j++) {
            if (s[j] == '1') {
                male_bm[i] |= (1 << j);
                female_bm[j] |= (1 << i);
            }
        }
        impos_male |= male_bm[i];
    }
    
    for (int j = 0; j < m; j++) {
        impos_female |= female_bm[j];
    }
    
    if (impos_male != female_max || impos_female != male_max) {
        cout << -1;
        return 0;
    }
    
    int marriage = n + m;
    
    vector<pair<int, int>> mdp(1 << n);
    vector<pair<int, int>> fdp(1 << m);
    
    for (int bx = 0; bx < (1 << n); bx++) {
        int bits = 0;
        int x = bx;
        int i = 0;
        int bm = 0;
        while (x) {
            if (x % 2) {
                bits++;
                bm |= male_bm[i];
            }
            x /= 2;
            i++;
        }
        mdp[bx] = make_pair(bits, bm);
    }
    
    for (int by = 0; by < (1 << m); by++) {
        int bits = 0;
        int y = by;
        int i = 0;
        int bm = 0;
        while (y) {
            if (y % 2) {
                bits++;
                bm |= female_bm[i];
            }
            y /= 2;
            i++;
        }
        fdp[by] = make_pair(bits, bm);
    }
    
    for (int bx = 0; bx < (1 << n); bx++) {
        for (int by = 0; by < (1 << m); by++) {
            pair<int, int> p1 = mdp[bx];
            pair<int, int> p2 = fdp[by];
            
            if ((bx | fdp[by].second) == male_max && (by | mdp[bx].second) == female_max) {
                marriage = min(marriage, fdp[by].first + mdp[bx].first);
            }
        }
    }
    
    cout << marriage;
}
