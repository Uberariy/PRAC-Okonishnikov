l = input()
a = []
flag = 1
while flag or l[0] != "-":
    flag = 0
    l = input()
    a.append(list(l))


Pr = [[]]
i = 0
sum = 0
while a[i][0] != "-":
    for j in range(len(l)):
        if a[i][j] == "#":
            k = i+1
            while a[k][j] == "#":
                k += 1
            p = j+1
            while a[i][p] == "#":
                p += 1
            #print(i,j,k-1,p-1)
            for h in range(i, k):
                for g in range(j, p):
                    a[h][g] = "."
            sum += 1
    i += 1

print(sum)