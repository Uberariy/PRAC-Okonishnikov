n = int(input())
a = 1000
while 2 <= a <= 1000:
    b = a**2
    i = 2
    while b <= 1000000:
        if n == b:
            a = 0
            break
        i += 1
        b = a**i
    a -= 1
else:
    if a==-1:
        print("YES")
    else:
        print("NO")