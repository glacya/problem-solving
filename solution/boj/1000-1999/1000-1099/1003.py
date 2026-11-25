# BOJ 1003 - 피보나치 함수
# 난이도: Silver III
# 푼 날짜: 2020-08-15
# https://www.acmicpc.net/problem/1003

for _ in range(int(input())):
    l = int(input())
    t = [1, 0]
    for k in range(l):
        s = t[0]+t[1]
        t[0] = t[1]
        t[1] = s
    print(t[0], t[1])
