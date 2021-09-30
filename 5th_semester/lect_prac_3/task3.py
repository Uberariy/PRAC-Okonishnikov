l = list(eval(input()))
Pr = [[]]
if l[0] != "-":
    l = list(eval(input()))
    Pr.append(l)
print(Pr)


i = 0
sum = 0
a = "."
while a[i][j] != "-":
    for j in range(len(l)):
        a = input()
        if a[i][j] == "#" and [i, j] not in Pr:
            k = i
            while a[k][j] == "#":
                k += 1
            p = i
            while a[i][p] == "#":
                p += 1
            for h in range(i, k):
                for g in range(j, p):
                    Pr.append([h, g])
            sum += 1

print(sum)