from itertools import *
from random import *
def joinseq(*l2):
    l = list(l2)
    print(l, l2)
    d = dict()
    for i in l:
        d[i] = 0
    while True:
        m = min([i[d[i]] for i in l])
        yield m
        p = [i[d[i]] for i in l].index(m)
        if d[l[p]]+1 < len(l[p]):
            d[l[p]] += 1
        else:
            del d[l[p]]
            l.pop(p)



import time
tick = time.perf_counter()
print("".join(joinseq("abs", "qr", "azt")))
print(time.perf_counter()-tick)