from itertools import *
def chainslice(a, b, *l):
    for i in islice(chain.from_iterable(l), a, b):
        yield i
    pass
#import time
#tick = time.perf_counter()

#print(time.perf_counter()-tick)