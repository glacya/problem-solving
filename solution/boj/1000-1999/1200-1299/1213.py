# BOJ 1213 - 팰린드롬 만들기
# 난이도: Silver III
# 푼 날짜: 2020-08-09
# https://www.acmicpc.net/problem/1213

a = input()
st = set()
ls = []
for let in a:
    if let in st:
        st.discard(let)
    else:
        st.add(let)
        ls.append(let)
if len(st) < 2:
    ls.sort()
    n = ""
    mid = ""
    for i in ls:
        if i in st:
            mid = i
            st.discard(i)
            continue
        n += i
    n += mid + n[::-1]
    print(n)
else:
    print("I'm Sorry Hansoo")
