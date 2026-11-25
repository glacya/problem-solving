# BOJ 1076 - 저항
# 난이도: Bronze II
# 푼 날짜: 2020-08-09
# https://www.acmicpc.net/problem/1076

k = ["black","brown","red","orange","yellow",
     "green","blue","violet","grey","white"]

a = input()
b = input()
c = input()
print((10*k.index(a)+k.index(b))*10**(k.index(c)))
