n = int(input())
i = n
while i<n+3:
    j = n
    while j<n+3:
        s = i*j
        c = 0
        while s!=0:
            c += s%10
            s = s//10
        if c == 6:
            print(i,"*",j,"=:=)", sep="", end = " ")
        else:
            print(i,"*",j,"=",i*j, sep="", end = " ")
        j += 1
    print("")
    i += 1