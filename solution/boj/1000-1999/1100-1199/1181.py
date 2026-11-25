# BOJ 1181 - 단어 정렬
# 난이도: Silver V
# 푼 날짜: 2020-08-12
# https://www.acmicpc.net/problem/1181

import heapq

n = int(input())
d = []
for i in range(n):
    u = input()
    if (len(u),u) not in d:
        heapq.heappush(d, (len(u),u))
while d != []:
    print(heapq.heappop(d)[1])
