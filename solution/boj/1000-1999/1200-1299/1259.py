# BOJ 1259 - 팰린드롬수
# 난이도: Bronze I
# 푼 날짜: 2020-08-09
# https://www.acmicpc.net/problem/1259

do = []
while True:
    a= input()
    if a == "0":
        break
    do.append(a)
for num in do:
    yes = True
    for i in range(len(num)//2):
        if num[i] != num[-(i+1)]:
            yes = False
            break
    if yes:
        print("yes")
    else:
        print("no")
