// BOJ 1533 - 길의 개수
// 난이도: Platinum III
// 푼 날짜: 2023-12-07
// https://www.acmicpc.net/problem/1533

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 xll;


typedef vector<vector<ll>> matrix;
ll mod = 1000003;

matrix mul(matrix &m1, matrix &m2) {
    int n = m1.size();
    int m = m1[0].size();
    int l = m2[0].size();
    matrix m3(n, vector<ll>(l));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < l; j++) {
            for (int k = 0; k < m; k++) {
                m3[i][j] += m1[i][k] * m2[k][j];
                m3[i][j] %= mod;
            }
        }
    }
    
    return m3;
}

matrix add(matrix &m1, matrix &m2) {
    int n = m1.size();
    matrix m3(n, vector<ll>(n));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m3[i][j] = (m1[i][j] + m2[i][j]) % mod;
        }
    }
    
    return m3;
}

int main() {
    fastio;
    
    int n, s, e, t;
    cin >> n >> s >> e >> t;
    
    vector<matrix> base(5, matrix(n, vector<ll>(n)));
    
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        
        for (int j = 0; j < n; j++) {
            if (s[j] != '0')
                base[s[j] - '1'][i][j] = 1;
        }
    }
    
    vector<matrix> first_matrix(5, matrix(n, vector<ll>(n)));
    
    for (int i = 0; i < n; i++) {
        first_matrix[0][i][i] = 1;
    }
    
    for (int x = 1; x < 5; x++) {
        for (int i = 0; i < x; i++) {
            matrix m = mul(base[i], first_matrix[x - i - 1]);
            first_matrix[x] = add(first_matrix[x], m);
        }
    }
    
    vector<vector<ll>> large(n * 5, vector<ll>(n * 5));
    vector<vector<ll>> column(n * 5, vector<ll>(n));
    for (int x = 0; x < 5; x++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                large[i][x * n + j] = base[x][i][j];
                column[(4 - x) * n + i][j] = first_matrix[x][i][j];
            }
        }
    }
    
    for (int x = 0; x < 4; x++) {
        for (int i = 0; i < n; i++) {
            large[(x + 1) * n + i][x * n + i] = 1;
        }
    }
    
    vector<vector<ll>> iden(n * 5, vector<ll>(n * 5));
    
    for (int i = 0; i < n * 5; i++) {
        iden[i][i] = 1;
    }
    
    while (t) {
        if (t % 2) iden = mul(large, iden);
        
        large = mul(large, large);
        t /= 2;
    }
    
    vector<vector<ll>> result = mul(iden, column);
    
    cout << result[4 * n + s - 1][e - 1];
}
