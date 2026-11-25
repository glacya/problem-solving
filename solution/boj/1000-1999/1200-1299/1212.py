# BOJ 1212 - 8진수 2진수
# 난이도: Bronze II
# 푼 날짜: 2020-08-09
# https://www.acmicpc.net/problem/1212

def tobin(o):
    if o == "0":
        print("0")
        return
    bi = ""
    for let in o:
        get = format(int(let),"03b")
        bi = bi + get
    while bi[0] == "0":
        bi=bi[1:]
    print(bi)

tobin(input())
