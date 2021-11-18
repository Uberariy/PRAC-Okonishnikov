from itertools import *
from random import *
def randomes(l):
    if not hasattr(l,'__next__'):
        while True:
            for i in l:
                yield randint(*i)
    else:
        while True:
            o = []
            while True:
                b = next(l, "@@")
                if b=="@@":
                    break
                o.append(tuple(b))
            while True:
                for i in o:
                    yield randint(*i)
#import time
#tick = time.perf_counter()

#print(time.perf_counter()-tick)