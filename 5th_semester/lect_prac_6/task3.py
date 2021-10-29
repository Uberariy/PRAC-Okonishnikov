a = input().split(" ")
d = dict()
while len(a) != 1:
    d.setdefault(a[0], []).append(a[1])
    a = input().split(" ")

ways = {a[0]}
exitt = input()
#print(d)
#nv_map = dict((d[k], k) for i in d for k in i)
#print(nv_map)

while True:
    chways = ways
    tmpways = set()
    for i in chways:
        if i in d:
            for j in d[i]:
                tmpways.add(j)
                pass
    #print(chways, ways)
    ways = tmpways | ways
    if chways == ways:
        break

if exit in ways:
    print("YES")
else:
    print("NO")