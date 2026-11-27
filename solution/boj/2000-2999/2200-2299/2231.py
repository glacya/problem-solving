# BOJ 2231 - 분해합
# 난이도: Bronze II
# 푼 날짜: 2020-08-16
# https://www.acmicpc.net/problem/2231

n = int(input())
ans = 0
for i in range(n-9*len(str(n)), n):
    if i < 0:
        continue
    k = str(i)
    a = 0
    for l in k:
        a += int(l)
    if a+i == n:
        ans = i
        break
print(ans)
