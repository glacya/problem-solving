# BOJ 1157 - 단어 공부
# 난이도: Bronze I
# 푼 날짜: 2020-08-10
# https://www.acmicpc.net/problem/1157

a = {}
b = input().lower()
for l in b:
    if a.__contains__(l):
        a[l] += 1
    else:
        a[l] = 1
j = False
t = None
for u in a:
    if t is None:
        t = u
        continue
    if a[u]>a[t]:
        t = u
        j = False
    elif a[u] == a[t]:
        j=True
if j:
    print("?")
else:
    print(t.upper())
