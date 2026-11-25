// BOJ 1197 - 최소 스패닝 트리
// 난이도: Gold IV
// 푼 날짜: 2023-03-31
// https://www.acmicpc.net/problem/1197

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Edge {
public:
    int A;
    int B;
    int weight;
    
    Edge() {
        this->A = 0;
        this->B = 0;
        this->weight = 0;
    }
    
    Edge(int a, int b, int w) {
        this->A = a;
        this->B = b;
        this->weight = w;
    }
};

int parent[10001];
Edge edges[100000];

int find_parent(int v) {
    if (v == parent[v]) return v;
    else return parent[v] = find_parent(parent[v]);
}

void union_parent(int v1, int v2) {
    int p1 = find_parent(v1);
    int p2 = find_parent(v2);
    
    if (p1 != p2) parent[p2] = p1;
}

bool comp(const Edge &e1, const Edge &e2) {
    return e1.weight < e2.weight;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int V, E;
    cin >> V >> E;
    
    // Kruskal's algorithm
    for (int i = 0; i < E; i++) {
        int A, B, C;
        cin >> A >> B >> C;
        edges[i] = Edge(A, B, C);
    }
    
    for (int i = 1; i <= V; i++) {
        parent[i] = i;
    }
    
    sort(edges, edges + E, comp);
    
    int weight_sum = 0;
    int vertex_count = 1;
    for (int i = 0; i < E; i++) {
        Edge &edge = edges[i];
        int parent_A = find_parent(edge.A);
        int parent_B = find_parent(edge.B);
        if (parent_A != parent_B) {
            union_parent(parent_A, parent_B);
            weight_sum += edge.weight;
            vertex_count++;
        }
        if (vertex_count == V) break;
    }
    
    cout << weight_sum;

    return 0;
}
