// BOJ 16926 - 배열 돌리기 1
// 난이도: Gold V
// 푼 날짜: 2026-11-28
// https://www.acmicpc.net/problem/16926

#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using ld = long double;
using xll = __int128;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, r;
    cin >> n >> m >> r;

    bool is_x_min_axis = n <= m;
    int min_side = is_x_min_axis ? n : m;
    int max_side = n + m - min_side;

    vector<vector<int>> rotated(n, vector<int>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int min_pivot = is_x_min_axis ? i : j;
            int max_pivot = i + j - min_pivot;
            int min_axis_radius = max(min_pivot - min_side / 2 + 1, min_side / 2 - min_pivot);
            int max_axis_radius = max(1, min_side / 2 + 1 - min(max_pivot + 1, max_side - max_pivot));
            int ring_radius = max(min_axis_radius, max_axis_radius);

            int ring_min_length = 2 * ring_radius;
            int ring_max_length = max_side - min_side + ring_min_length;
            int er = r % (2 * (ring_min_length + ring_max_length) - 4);

            int x_min = is_x_min_axis ? (n / 2 - ring_radius) : ((n - ring_max_length) / 2);
            int x_max = n - 1 - x_min;
            int y_min = !is_x_min_axis ? (m / 2 - ring_radius) : ((m - ring_max_length) / 2);
            int y_max = m - 1 - y_min;

            int x = i;
            int y = j;

            while (er > 0) {
                // On which component is (i, j)?
                if (x == x_min && y != y_min) {
                    // Upper.
                    int dy = min(er, y - y_min);
                    er -= dy;
                    y -= dy;
                }
                else if (y == y_min && x != x_max) {
                    // Left.
                    int dx = min(er, x_max - x);
                    er -= dx;
                    x += dx;
                }
                else if (x == x_max && y != y_max) {
                    // Lower.
                    int dy = min(er, y_max - y);
                    er -= dy;
                    y += dy;
                }
                else {
                    // Right.
                    int dx = min(er, x - x_min);
                    er -= dx;
                    x -= dx;
                }
            }

            // cout << "Radius " << ring_radius << ": X = [" << x_min << ", " << x_max << "], Y = [" << y_min << ", " << y_max << "]\n";

            // rotated[i][j] = ring_radius;
            cin >> rotated[x][y];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << rotated[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
