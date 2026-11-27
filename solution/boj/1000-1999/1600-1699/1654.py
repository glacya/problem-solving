# BOJ 1654 - 랜선 자르기
# 난이도: Silver II
# 푼 날짜: 2020-08-15
# https://www.acmicpc.net/problem/1654

def cal(ls, pre):
    z = 0
    for v in ls:
        z += v//pre
    return z

k, n = map(int,input().split())
val = []
num = 0
for i in range(k):
    val.append(int(input()))
    num += val[-1]
m = num//n
k = num//(n+1)
if k == 0:
    k = 1
j = 2
while cal(val, k) < n:
    m = k
    k = num//(n+j)
    j += 1
while True:
    tar = int(m - (m-k)/2)
    if tar == k:
        break
    if cal(val, tar) >= n:
        k = tar
    else:
        m = tar
if cal(val, tar) == cal(val, tar + 1):
    tar += 1
print(tar)
