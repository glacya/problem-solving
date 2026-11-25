# BOJ 1025 - 제곱수 찾기
# 난이도: Gold V
# 푼 날짜: 2023-10-17
# https://www.acmicpc.net/problem/1025

import math

n, m = map(int, input().split())

arr = []

for _ in range(n):
    arr.append(input())
    
max_square = -1
for x in range(n):
    for y in range(m):
        for sx in range(-n + 1, n):
            for sy in range(-m + 1, m):
                int_string = ""
                for i in range(9):
                    nx = x + i * sx
                    ny = y + i * sy
                    
                    if nx < 0 or ny < 0 or nx >= n or ny >= m:
                        break
                    
                    int_string += arr[nx][ny]
                    value = int(int_string)
                    sqrted = math.floor(math.sqrt(value))
                    
                    if sqrted * sqrted == value:
                        max_square = max(max_square, value)
        
print(max_square)
