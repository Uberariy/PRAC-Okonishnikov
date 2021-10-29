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

def Recur(depth, res):
    if depth > 0:
        c = res[-3::]
        curres = res
        while len(D[tuple([c[1], c[2]])]) == 1:
            depth -= 1
            c = [c[1], c[2], D[tuple([c[1], c[2]])][0]]
            curres.append(c[2])
        else:
            depth -= 1
            for j in range(len(D[tuple([c[1], c[2]])])):
                Recur(depth, curres+[D[tuple([c[1], c[2]])][j]])
    else:
        print(" ".join(res[0:N]))
        sys.exit()


Recur(N, c)
