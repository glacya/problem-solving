# BOJ 1764 - 듣보잡
# 난이도: Silver IV
# 푼 날짜: 2021-02-23
# https://www.acmicpc.net/problem/1764

import sys
input = sys.stdin.readline

save = set()
l = list()

n, k = map(int, input().rstrip().split())

for i in range(n):
    save.add(input().rstrip())

for i in range(k):
    s = input().rstrip()
    if s in save:
        l.append(s)

l = sorted(l)

print(len(l))
for i in l:
    print(i)
