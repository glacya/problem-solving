# BOJ 1018 - 체스판 다시 칠하기
# 난이도: Silver III
# 푼 날짜: 2020-08-12
# https://www.acmicpc.net/problem/1018

a, b = map(int, input().split())
data = []
for i in range(a):
    data.append(input())
mini = 64
for j in range(a - 7):
    for k in range(b - 7):
        val = 0
        for l in range(j, j+8):
            it = "W"
            if l%2 == 0:
                it = "B"
            for m in range(k, k+8):
                if data[l][m] == it:
                    val += 1
                if it == "B":
                    it = "W"
                else:
                    it = "B"
        mini = min(min(val, 64-val), mini)
print(mini)
