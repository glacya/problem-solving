// BOJ 1069 - 집으로
// 난이도: Gold III
// 푼 날짜: 2023-10-07
// https://www.acmicpc.net/problem/1069

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

double sq(double x) {
    return x * x;
}

int main() {
    fastio;
    
    double x, y, d, t;
    cin >> x >> y >> d >> t;
    
    double str_dist = sqrt(sq(x) + sq(y));
    
    double ans = str_dist;
    
    cout << fixed;
    cout.precision(12);
    if (t >= d) {
        cout << str_dist;
        return 0;
    }
    
    if (d == str_dist) {
        ans = min(ans, t);
    }
    else if (d < str_dist) {
        double c = 0.0;
        for (double x = 0.0; x < str_dist; x += d) {
            ans = min(ans, c * t + str_dist - x);
            c += 1.0;
        }
        ans = min(ans, c * t);
    }
    else {
        ans = min(ans, 2.0 * t);
        ans = min(ans, t + d - str_dist);
    }
    
    cout << ans;
}
