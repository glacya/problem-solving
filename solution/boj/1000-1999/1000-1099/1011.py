# BOJ 1011 - Fly me to the Alpha Centauri
# 난이도: Gold V
# 푼 날짜: 2020-08-16
# https://www.acmicpc.net/problem/1011

for _ in range(int(input())):
    a,b = map(int,input().split())
    if b - a < 4:
        print(b-a)
        continue
    k = int((b-a)**0.5)
    l = k + 1
    ans = 2*k
    if b - a == k**2:
        ans -= 1
    elif b - a > k**2 + (l**2-k**2)//2:
        ans += 1
    print(ans)
