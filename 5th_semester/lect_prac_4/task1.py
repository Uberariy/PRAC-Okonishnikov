def divdigit(a):
    p = a
    n = 0
    while p>9:
        d = p%10
        if d != 0 and a%d == 0:
            n += 1
        p = p//10
    else:
        d = p%10
        if d != 0 and a%d == 0:
            n += 1      
    return n

#print(divdigit(100000000001))