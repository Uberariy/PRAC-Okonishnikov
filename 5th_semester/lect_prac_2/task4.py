a = k = int(input())
s = 0
c = 1
while True:
    if k*10*(10**s-k)%(10*k-1)==0:
        break
    s += 1
    c *= 10
    a = int(k*10*(10**s-k)//(10*k-1)+k)
print(a)