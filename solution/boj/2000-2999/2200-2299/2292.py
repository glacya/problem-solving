# BOJ 2292 - 벌집
# 난이도: Bronze II
# 푼 날짜: 2020-08-14
# https://www.acmicpc.net/problem/2292

import sys
input = sys.stdin.readline

n = int(input().rstrip())
m = 1
c = 0
while True:
    if n == 1 and m == 1:
        c += 1
        break
    if n == 0:
        break
    if 0 < n < m:
        c += 1
        break
    n -= m
    if m == 1:
        m = 6
    else:
        m += 6
    c += 1
print(c)
