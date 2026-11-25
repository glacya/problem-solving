# BOJ 1016 - 제곱 ㄴㄴ 수
# 난이도: Gold I
# 푼 날짜: 2020-08-12
# https://www.acmicpc.net/problem/1016

def main():
    d = set()
    low, hi = map(int,input().split())
    p = prime()
    for i in p:
        if i > hi:
            break
        k = (low//i+1)*i
        if low%i == 0:
            k -= i
        while k <= hi:
            d.add(k)
            k += i
    if hi > p[-1]:
        u = 1003
        while u**2 <= hi:
            if low % u**2 == 0:
                d.add(low)
            else:
                z = (low//(u**2)+1)*u**2
                if z <= hi:
                    d.add(z)
            u += 2
    print(hi-low+1-len(d))

def prime():
    t = [4,9]
    v = 5
    while v < 1000:
        for pr in t:
            if v < pr:
                t.append(v**2)
                break
            elif v**2 % pr == 0:
                break
        v += 2
    return t
    
main()
