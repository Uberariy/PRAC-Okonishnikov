import sys
import random
N = int(input())
Txt = sys.stdin.read().replace("\n    ", " @ ").split()
for i in range(len(Txt)):
    if Txt[i] == "@":
        Txt[i] = "\n    "
#D = {tuple([Txt[i], Txt[i+1]]): Txt[i+2] for i in range(len(Txt)-2)}
D = dict()
for i in range(len(Txt)-2):
    D.setdefault(tuple([Txt[i], Txt[i+1]]), []).append(Txt[i+2])
a = random.choice(list(D.items()))
c = [a[0][0], a[0][1], a[1][0]]
#c = ["Мой", "дядя", "самых"]

res = [c]
CurRes = []
while len(res[-1]) < N:
    #print(len(res))
    for i in range(len(res)//2):
        del res[i]
    c = res[-1][-3::]
    curres = res[-1]
    if c[1] == "в" and c[2] == 'нее!"':
        del res[-1]
        continue
    while (c[1] != "в" or c[2] != 'нее!"') and len(D[tuple([c[1], c[2]])]) == 1:
        c = [c[1], c[2], D[tuple([c[1], c[2]])][0]]
        curres.append(c[2])
    else:
        if c[1] == "в" and c[2] == 'нее!"':
            del res[-1]
            continue
        for j in random.sample(range(0,len(D[tuple([c[1], c[2]])])),len(D[tuple([c[1], c[2]])])):
            res += [curres+[D[tuple([c[1], c[2]])][j]]]

print(" ".join(res[-1][0:N]))