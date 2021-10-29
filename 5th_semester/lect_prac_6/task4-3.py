import sys
import random
N = int(input())
Txt = sys.stdin.read().replace("\n    ", " @ ").split()
D = {tuple([Txt[i], Txt[i+1]]): Txt[i+2] for i in range(len(Txt)-2)}

i = 0
while i != N-1:
    a = random.choice(list(D.items()))
    c = [a[0][0], a[0][1], a[1]]
    Res = ""
    nextry = 0
    #print(c)
    for i in range(N):
        if nextry:
            break
        if c[0] == "@":
            Res+="\n    "+" "
        else:
            Res+=c[0]+" "
        if c[1] == "в" and c[2] == 'нее!"':
            nextry = 1
        else:
            c = [c[1], c[2], D[tuple([c[1], c[2]])]]
print(Res)
