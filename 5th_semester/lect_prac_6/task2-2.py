def eightcheck(a):
    return (a-7)//8 == (a-7)/8

def sumof2(a):
    N = int(a**0.5)+1
    for i in range(1,N):
        for j in range(1,N):
            if i**2+j**2==a:
                print(a, i, j)
                return True
    return False

seq = set(eval(input()))

count = 0
for a in seq:
    #print(" ")
    while True:
        #print(a)
        if eightcheck(a):
            break
        elif a//4 == a/4:
            a = a//4
        else:
            if not sumof2(a) and round(a**0.5)!=a**0.5:
                count += 1
            break

print("wrong",count)
#print(eightcheck(13))