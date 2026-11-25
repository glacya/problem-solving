# BOJ 1110 - 더하기 사이클
# 난이도: Bronze I
# 푼 날짜: 2020-08-14
# https://www.acmicpc.net/problem/1110

a = input()
if len(a) == 1:
    a = "0" + a
b = a
step = 0
while True:
    k = (int(a[0]) + int(a[1]))%10
    a = a[1] + str(k)
    step += 1
    if b == a:
        break
print(step)
