a = input().split(" ")
l = []
while len(a) != 1:
    l.append(a)
    l.append([a[1], a[0]])
    a = input().split(" ")

ways = {a[0]}
exitt = str(input())
d = dict()
for i in list(set(tuple(i) for i in l)):
    d.setdefault(i[0], []).append(i[1])

oldways = ways
while True:
    newways = set()
    ldel = []
    for i in oldways:
        if i in d:
            for j in d[i]:
                newways.add(j)
            ldel.append(i)
    for i in ldel:
        del d[i]
    oldways = newways
    if exitt in newways:
        print("YES")
        break
    if newways == set():
        print("NO")
        break