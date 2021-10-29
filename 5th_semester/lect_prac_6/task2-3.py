from math import *
seq = set(eval(input()))

#import time
#tick = time.perf_counter()
sq = {3}
a = max(seq)
for i in range(1,int(sqrt(a)+1)):
    for j in range(i,int(sqrt(a-i*i)+1)):
        for k in range(j,int(sqrt(a-i*i-j*j)+1)):
            sq.add(i*i+j*j+k*k)

#print(time.perf_counter()-tick)
print(len(sq&seq))