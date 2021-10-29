seq = set(eval(input()))

count = 0
for a in seq:
    next=0
    Ni = int(a**0.5)+1
    for i in range(1,Ni):
        if next:
            break
        Nj = int((a-i**2)**0.5)+1
        for j in range(i,Nj):
            if next:
                break
            Nk = int((a-i**2-j**2)**0.5)+1
            for k in range(j,Nk):
                if i**2+j**2+k**2==a:
                    count+=1
                    #print(a, i, j, k)
                    next=1
                if next:
                    break

print(count)