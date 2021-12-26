from itertools import *

n = int(input())
s = set()
if n >= 6:
    for i in product('TOR', repeat=n-6):
        T = ("TOR", "TOR", *[j for j in chain.from_iterable([i])])
        a = permutations(T, n-4)
        for i in a:
            o = "".join(i)
            if len("".join(o.split("TOR"))) == n-6:
                s.add(o)
    print(*sorted(s), sep=", ")
else:
    print("")

