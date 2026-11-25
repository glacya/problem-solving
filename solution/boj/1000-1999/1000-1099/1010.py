# BOJ 1010 - 다리 놓기
# 난이도: Silver V
# 푼 날짜: 2023-10-17
# https://www.acmicpc.net/problem/1010

comb = [[0 for i in range(31)] for i in range(31)]

for i in range(31):
    comb[i][0] = 1;
    comb[i][i] = 1;

for i in range(31):
    for j in range(1, i):
        comb[i][j] = comb[i - 1][j - 1] + comb[i - 1][j]

t = int(input())

for _ in range(t):
    n, m = map(int, input().split())
    print(comb[m][n])
