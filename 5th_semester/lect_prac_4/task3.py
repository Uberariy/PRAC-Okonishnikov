def Det(k, l, M, o):
    Res = 0
    u = -1
    for i in range(o):
        if i not in l:
            if o-1 == k:
                return(M[k][i])
            u += 1
            Res += (-1)**(u)*M[k][i]*Det(k+1,l+[i],M,o)
    return(Res)
    
M = tuple(eval(input()))
print(Det(0, [], M, 4))
