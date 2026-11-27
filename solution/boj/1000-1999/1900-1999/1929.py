# BOJ 1929 - 소수 구하기
# 난이도: Silver III
# 푼 날짜: 2021-02-18
# https://www.acmicpc.net/problem/1929

m, n = map(int, input().split())

count = 0
p = [2,3]
v = 5
while v <= n:
    re = True
    for i in p:
        if v < i**2:
            break
        if v % i == 0:
            re = False
            break
    if re:
        p.append(v)
    v += 2

for k in p:
    if k >= m and k <= n:
        print(k)
