# BOJ 1085 - 직사각형에서 탈출
# 난이도: Bronze III
# 푼 날짜: 2020-08-12
# https://www.acmicpc.net/problem/1085

x, y, w, h = map(int, input().split())
print(min([x, y, w-x, h-y]))
