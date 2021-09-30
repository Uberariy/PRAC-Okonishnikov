n = list(eval(input()))

M = [[-1 for x in range(n[0])] for y in range(n[1])]
Dir = 0 

a = 0
i = 0
j = 0
while True:
    if n[1] == n[0] == 1:
        M[0][0] = 0
        break
    if i == n[1] or j == n[0] or i == -1 or j == -1 or M[i][j] >= 0:
        if Dir%4 == 0:
            j -= 1
        elif Dir%4 == 1:
            i -= 1
        elif Dir%4 == 2:
            j += 1
        elif Dir%4 == 3:
            i += 1
        Dir += 1
        fl = 1
    else:
        M[i][j] = a
        a = (a+1)%10
        fl = 0
    if Dir%4 == 0:
        j += 1
    elif Dir%4 == 1:
        i += 1
    elif Dir%4 == 2:
        j -= 1
    elif Dir%4 == 3:
        i -= 1
    if fl and i != n[1] and j != n[0] and i != -1 and j != -1 and M[i][j] >= 0:
        break;


for i in range(0, n[1], 1):
    for j in range(0, n[0], 1):
        print(M[i][j], end=' ')
    print("")