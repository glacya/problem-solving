# BOJ 1620 - 나는야 포켓몬 마스터 이다솜
# 난이도: Silver IV
# 푼 날짜: 2020-08-17
# https://www.acmicpc.net/problem/1620

import sys
input = sys.stdin.readline

num_name = {}
n, m = map(int,input().strip().split())
for i in range(n):
    num_name[i+1] = input().strip()
name_num = dict((value, key) for key, value in num_name.items())
for _ in range(m):
    ask = input().strip()
    try:
        print(num_name[int(ask)])
    except:
        print(name_num[ask])
