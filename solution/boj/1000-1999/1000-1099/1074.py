# BOJ 1074 - Z
# 난이도: Gold V
# 푼 날짜: 2020-08-17
# https://www.acmicpc.net/problem/1074

import sys
input = sys.stdin.readline


def visit(n,r,c):
    if n == 0:
        return 1
    elif n == 1:
        value = 0
        if r == 1:
            value += 2
        if c == 1:
            value += 1
        return value
    else:
        if r >= 2**(n-1):
            if c >= 2**(n-1):
                return 2**(2*n)*3//4 + visit(n-1,r%(2**(n-1)),c%(2**(n-1)))
            else:
                return 2**(2*n)//2 + visit(n-1,r%(2**(n-1)),c)
        else:
            if c >= 2**(n-1):
                return 2**(2*n)//4 + visit(n-1,r,c%(2**(n-1)))
            else:
                return visit(n-1,r,c)


n,r,c = map(int,input().strip().split())
print(visit(n,r,c))
