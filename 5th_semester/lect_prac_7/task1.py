from itertools import *
def LookSay():
    l = [1]
    yield 1
    while True:
        i = 0
        j = 0
        o = []
        a = l[0]
        for i in l:
            if i!=a:
                o.append(j)
                yield j
                o.append(a)
                yield a
                j = 1
                a = i
            else:
                j += 1
        o.append(j)
        yield j
        o.append(a)
        yield a
        l = o
        #print(l)

#import time
#tick = time.perf_counter()
print(list(zip(LookSay(),range(50501))))
#print(time.perf_counter()-tick)