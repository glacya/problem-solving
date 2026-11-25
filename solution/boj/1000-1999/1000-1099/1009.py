# BOJ 1009 - 분산처리
# 난이도: Bronze II
# 푼 날짜: 2020-08-08
# https://www.acmicpc.net/problem/1009

t = int(input())
prob = []
for i in range(t):
    prob.append(input())
for i in range(t):
    base, power = map(int, prob[i].split())
    digit = power%4
    if digit == 0:
        digit = 4
    ans = str(base**digit)[-1]
    if ans == "0":
        ans = "10"
    print(ans)

