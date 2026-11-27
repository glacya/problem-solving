// BOJ 2250 - 트리의 높이와 너비
// 난이도: Gold II
// 푼 날짜: 2023-08-03
// https://www.acmicpc.net/problem/2250

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Children {
  public:
    int left = -1;
    int right = -1;
    
    Children() {}
    Children(int l, int r) {
        this->left = l;
        this->right = r;
    }
};

int tree_pos = 1;
vector<Children> tree;
vector<int> parent;
vector<int> leftmost;
vector<int> rightmost;

void traverse(int node, int depth) {
    Children &c = tree[node];
    
    if (c.left > 0) {
        traverse(c.left, depth + 1);
    }
    
    if (leftmost[depth] > tree_pos) {
        leftmost[depth] = tree_pos;
    }
    
    rightmost[depth] = tree_pos++;
    
    if (c.right > 0) {
        traverse(c.right, depth + 1);
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int N;
    cin >> N;
    
    tree = vector<Children>(N + 1);
    parent = vector<int>(N + 1);
    leftmost = vector<int>(N + 1, 1e9);
    rightmost = vector<int>(N + 1, -1);
    
    for (int i = 0; i < N; i++) {
        int x, l, r;
        cin >> x >> l >> r;
        tree[x] = Children(l, r);
        
        if (l >= 0) parent[l] = x;
        if (r >= 0) parent[r] = x;
    }
    
    int root = 1;
    while (parent[root]) {
        root = parent[root];
    }
    
    traverse(root, 1);
    
    int max_width = 0;
    int ans_depth = 1;
    for (int i = 1; i <= N; i++) {
        int width = rightmost[i] - leftmost[i] + 1;
        if (width > 0 && width > max_width) {
            max_width = width;
            ans_depth = i;
        }
    }
    
    cout << ans_depth << " " << max_width;

    return 0;
}
