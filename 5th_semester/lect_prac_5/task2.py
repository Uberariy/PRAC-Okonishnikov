N, M = eval(input())
d = len(str(N))
p = len(str(N*N))
o = d*2+p+6
k = 1
while o <= M:
    k += 1
    o += d*2+p+6+3
u = k
print("="*M)
while u<N+k:
    for i in range(1,N+1):
        for j in range(1+u-k,min(u,N+1)):
            print("{:"f"{d}""} * {:<"f"{d}""} = {:<"f"{p}""}".format(j,i,i*j), end="")
            if j != min(u,N+1)-1:
                print(" | ", end="")
        print("")
    u += k-1
    print("="*M)