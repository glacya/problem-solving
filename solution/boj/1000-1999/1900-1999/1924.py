# BOJ 1924 - 2007년
# 난이도: Bronze I
# 푼 날짜: 2020-08-15
# https://www.acmicpc.net/problem/1924

d = [31,28,31,30,31,30,31,31,30,31,30,31]
p = ["SUN","MON","TUE","WED","THU","FRI","SAT"]
x,y=map(int,input().split())
for i in range(x-1):
    y+=d[i]
print(p[y%7])
