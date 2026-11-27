# BOJ 1312 - 소수
# 난이도: Silver V
# 푼 날짜: 2020-08-14
# https://www.acmicpc.net/problem/1312

a, b, n = map(int,input().split())
for _ in range(n-1):
    a = (a*10)%b
    if a == 0:
        break
num = a*10//b
if n == 1:
    print(str(num)[-1])
else:
    print(num)
