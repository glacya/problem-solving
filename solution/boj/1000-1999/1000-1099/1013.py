# BOJ 1013 - Contact
# 난이도: Gold V
# 푼 날짜: 2023-10-17
# https://www.acmicpc.net/problem/1013

t = int(input())

for _ in range(t):
    s = input()
    n = len(s)
    arr = [False for _ in range(n)]
    
    if s[-1] == '0':
        print("NO")
        continue
    
    zs = 0
    ts = 0
    succ = True
    found_zero = False
    slist = []
    for i in range(n):
        if s[i] == '0' and not found_zero:
            found_zero = True
            zs = i
        elif found_zero and s[i] == '1':
            found_zero = False
            length = i - zs
            
            if length == 1:
                slist.append(s[ts:i - 1])
                ts = i + 1
    
    slist.append(s[ts:])
    
    # print(slist)
    
    for elem in slist:
        if len(elem) == 0:
            continue
        elif len(elem) >= 1 and len(elem) < 4:
            succ = False
            break
        
        if elem[0] == '0' or elem[1] == '1' or elem[-1] == '0':
            succ = False
            break
        
        isz = len(elem)
        curr_sz = 1
        for i in range(2, len(elem) - 1):
            if elem[i - 1] == elem[i]:
                curr_sz += 1
            else:
                # print(f"{i}th index, curr_sz = {curr_sz}")
                isz = min(isz, curr_sz)
                curr_sz = 1
        
        if isz == 1:
            succ = False
            break
    
    print("YES" if succ else "NO")
