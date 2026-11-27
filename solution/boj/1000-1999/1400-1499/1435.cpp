// BOJ 1435 - 네 점
// 난이도: Platinum V
// 푼 날짜: 2023-06-21
// https://www.acmicpc.net/problem/1435

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

double sq(double x) {
    return x * x;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    vector<vector<int>> dist(4, vector<int>(4));
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cin >> dist[i][j];
        }
    }
    
    vector<vector<vector<int>>> step = 
        {
            {
                {0, 1}, {0, 2}, {1, 2}
            },
            {
                {0, 1}, {0, 3}, {1, 3}
            },
            {
                {0, 2}, {0, 3}, {2, 3}
            },
            {
                {1, 2}, {1, 3}, {2, 3}
            }
        };
    
    bool possible = true;
    for (int i = 0; i < 4; i++) {
        vector<int> edges(3);
        for (int j = 0; j < 3; j++) {
            vector<int> &indices = step[i][j];
            edges[j] = dist[indices[0]][indices[1]];
        }
        sort(edges.begin(), edges.end());
        
        // cout << edges[0] << " " << edges[1] << " " << edges[2] << "\n";
        
        if (edges[0] + edges[1] < edges[2]) {
            possible = false;
            break;
        }
    }
    
    if (possible) {
        int i1 = 0, i2 = 1, i3 = 2;
        int maxdist = dist[0][1];
        for (int i = 0; i < 3; i++) {
            vector<int> &indices = step[0][i];
            if (dist[indices[0]][indices[1]] > maxdist) {
                i1 = indices[0];
                i2 = indices[1];
                maxdist = dist[i1][i2];
                i3 = 3 - i1 - i2;
            }
        }
        
        // Select the longest edge among AB, BC, CA to be AB
        double AB = (double) dist[i1][i2];
        double AC = (double) dist[i1][i3];
        double BC = (double) dist[i2][i3];
        double AH = (sq(AB) + sq(AC) - sq(BC)) / (2.0 * AB);
        // Distance between segment AB and point C
        double CH = sqrt(sq(AC) - sq(AH));
        
        double AD = (double) dist[i1][3];
        double BD = (double) dist[i2][3];
        double CD = (double) dist[i3][3];
        
        double AI = (sq(AD) - sq(BD) + sq(AB)) / (2.0 * AB);
        double DI = sqrt(sq(AD) - sq(AI));
        
        double maxCD = sqrt(sq(AI - AH) + sq(DI + CH));
        double minCD = sqrt(sq(AI - AH) + sq(DI - CH));
        
        // cout << "CH = " << CH << "\n";
        // cout << "DI = " << DI << "\n";
        // cout << "AH = " << AH << "\n";
        // cout << "AI = " << AI << "\n";
        // cout << "maxCD = " << maxCD << "\n";
        // cout << "minCD = " << minCD << "\n";
        // cout << "CD = " << CD << "\n";
        
        if (CD > maxCD || CD < minCD) {
            possible = false;
        }
    }
    
    cout << (possible ? "YES" : "NO");

    return 0;
}
