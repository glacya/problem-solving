# BOJ 1874 - 스택 수열
# 난이도: Silver II
# 푼 날짜: 2020-08-16
# https://www.acmicpc.net/problem/1874

import sys
input = sys.stdin.readline

n = int(input().rstrip())
stack = []
pivot = 0
pre = 0
output = ''
yes = True
for _ in range(n):
    k = int(input().rstrip())
    if pivot == 0 and pre == 0:
        pivot = k
        pre = k
        for i in range(pivot):
            output += '+\n'
            stack.append(i+1)
        stack.pop()
        output += '-\n'
    else:
        if k < pre:
            if k == stack.pop():
                output += '-\n'
            else:
                yes = False
                break
        else:
            for i in range(k-pivot):
                output += '+\n'
                stack.append(i+pivot+1)
            stack.pop()
            output += '-\n'
            pivot = k
            pre = k
if yes:
    print(output)
else:
    print("NO")
