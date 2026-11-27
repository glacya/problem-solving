# BOJ 1436 - 영화감독 숌
# 난이도: Silver V
# 푼 날짜: 2020-08-16
# https://www.acmicpc.net/problem/1436

n = int(input())
num = 666
c = 0
while n > c:
    if "666" in str(num):
        c += 1
    num += 1
print(num-1)
