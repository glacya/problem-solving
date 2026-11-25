# BOJ 1038 - 감소하는 수
# 난이도: Gold V
# 푼 날짜: 2023-10-17
# https://www.acmicpc.net/problem/1038

def rec(fd, l, n, arr):
    if l == 0:
        return ""
    
    if n == 0:
        return "0"
    
    # print(f"rec({fd}, {l}, {n})")
    acc = 0
    for i in range(0, fd + 1):
        acc += arr[l - 1][i]
        
        if n <= acc:
            return f"{i}" + rec(i - 1, l - 1, n - acc + arr[l - 1][i], arr)
    
    return "?!"

def find(n):
    acc = 0
    for i in range(0, 10):
        s = sum(arr[i])
        acc += s
        
        if n <= acc:
            print(rec(9, i + 1, n - acc + s, arr))
            return
    print(-1)

n = int(input())

arr = [[1 for _ in range(10)] for _ in range(10)]

for i in range(1, 9):
    for j in range(i + 1, 10):
        arr[i][j] = arr[i - 1][j - 1] + arr[i][j - 1]
        
    for j in range(0, i):
        arr[i][j] = 0

# print(arr)

if n > 1022:
    print(-1)
elif n == 1022:
    print(9876543210)
else:
    find(n + 1)

# for n in range(1025):
#     print("Case ", n)
#     find(n + 1)
