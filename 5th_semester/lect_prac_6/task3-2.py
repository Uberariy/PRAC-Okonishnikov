a = input().split(" ")
l = []
while len(a) != 1:
    l.append(a)
    l.append([a[1], a[0]])
    a = input().split(" ")

#import time
#tick = time.perf_counter()
ways = {a[0]}
exitt = str(input())
s = set(tuple(i) for i in l)
#print(s, ways, exitt)

oldways = ways
while True:
    newways = set()
    ldel = []
    for i in oldways:
        for j in s:
            if i == j[0] and j[1] not in ways:
                newways.add(j[1])
                ldel.append(j)
    for i in ldel:
        s.discard(i)
    oldways = newways
    #print("iter", len(newways), len(ways), len(s), len(ldel))
    if exitt in newways:
        print("YES")
        break
    if newways == set():
        print("NO")
        break
    ways = newways | ways
#print(time.perf_counter()-tick)