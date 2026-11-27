# BOJ 1978 - 소수 찾기
# 난이도: Bronze II
# 푼 날짜: 2020-08-13
# https://www.acmicpc.net/problem/1978

p = [2, 3]
v = 5
while v < 1000:
    re = True
    for pn in p:
        if v < pn**2:
            break
        if v % pn == 0:
            re = False
            break
    if re:
        p.append(v)
    v += 2

a = input()
b = list(map(int, input().split()))
c = 0
for num in b:
    if num in p:
        c += 1
print(c)
