# BOJ 1094 - 막대기
# 난이도: Silver V
# 푼 날짜: 2023-10-17
# https://www.acmicpc.net/problem/1094

n = int(input())

ans = 0
while n > 0:
    if n % 2 == 1:
        ans += 1
    n //= 2
    
print(ans)
