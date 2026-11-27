# BOJ 2164 - 카드2
# 난이도: Silver IV
# 푼 날짜: 2020-08-13
# https://www.acmicpc.net/problem/2164

import sys

n = int(sys.stdin.readline())
k = 1
d = {}
for i in range(n):
    d[i+1] = i+1

while len(d) > 1:
    d.pop(k)
    d[n+(k//2)+1] = d.pop(k+1)
    k += 2
print(d[2*n-1])
