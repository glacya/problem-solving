# BOJ 1160 - Random Number Generator
# 난이도: Platinum V
# 푼 날짜: 2021-07-04
# https://www.acmicpc.net/problem/1160

def mult(m1, m2, m):
    output = [[0, 0], [0, 0]]

    for i in range(2):
        for j in range(2):
            for k in range(2):
                output[i][j] += (m1[i][k] * m2[k][j]) % m
            output[i][j] = output[i][j] % m

    return output


m, a, c, x0, n, g = map(int, input().split())
acc = [[1, 0], [0, 1]]
base = [[a % m, 0], [c % m, 1]]

while n != 0:
    if n % 2 == 1:
        acc = mult(acc, base, m)
    base = mult(base, base, m)
    n = n // 2

value = ((acc[0][0] * (x0 % m)) % m + acc[1][0] % m) % m
print(value % g)
