# BOJ 1546 - 평균
# 난이도: Bronze I
# 푼 날짜: 2020-08-10
# https://www.acmicpc.net/problem/1546

a = int(input())
b = map(int, input().split())
v = 0
m = 0
for i in b:
    v += i
    if m < i:
        m = i
print(v/m*100/a)
