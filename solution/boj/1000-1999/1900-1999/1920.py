# BOJ 1920 - 수 찾기
# 난이도: Silver IV
# 푼 날짜: 2020-08-13
# https://www.acmicpc.net/problem/1920

n = int(input())
a = set(map(int,input().split()))
m = int(input())
b = map(int,input().split())
for i in b:
    if i in a:
        print(1)
    else:
        print(0)
