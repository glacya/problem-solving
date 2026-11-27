# BOJ 1330 - 두 수 비교하기
# 난이도: Bronze V
# 푼 날짜: 2020-08-10
# https://www.acmicpc.net/problem/1330

a,b=map(int,input().split())
if a>b:
    print(">")
elif a<b:
    print("<")
else:
    print("==")
