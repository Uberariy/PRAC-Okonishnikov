from itertools import *
from random import *
def joinseq(*l):
    def nextel(l):
        for i in l:
            yield i
    o = [nextel(i) for i in l]
    u = [next(i) for i in o]
    while len(o)>0:
        m = min(u)
        p = u.index(m)
        u[p] = next(o[p], "@@")
        if u[p]=="@@":
            o.pop(p)
            u.pop(p)
        yield m

#import time
#tick = time.perf_counter()

#print(time.perf_counter()-tick)