def fun(s, f):
    l = f.split("@")
    n = 0
    while True:
        n=s.find(l[0], n)
        m=n
        if n<0:
            return -1
        n += len(l[0]) + 1
        for k in l[1:]:
            if k != s[n:n+len(k)]:
                break
            n += len(k) + 1
        else:
            return m

s = str(input())
f = str(input())
print(fun(s, f))
