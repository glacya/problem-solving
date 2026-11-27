# BOJ 2108 - 통계학
# 난이도: Silver II
# 푼 날짜: 2020-08-16
# https://www.acmicpc.net/problem/2108

import sys
input = sys.stdin.readline

n = int(input().rstrip())
l = []
d = {}
j = []
s = 0
for _ in range(n):
    a = int(input().rstrip())
    l.append(a)
    s += a
    try:
        d[a] += 1
    except:
        d[a] = 1
l.sort()
e = d.values()
for v in d:
    if d[v] == max(e):
        j.append(v)
j.sort()
try:
    mode = j[1]
except:
    mode = j[0]
print(float.__round__(s/n))
print(l[n//2])
print(mode)
print(l[-1]-l[0])
