import sys
import random
N = int(input())
Txt = sys.stdin.read().replace("\n    ", " @ ").split()
D = {tuple([Txt[i], Txt[i+1]]): Txt[i+2] for i in range(len(Txt)-2)}
a = list(D.items())[4000]
c = [a[0][0], a[0][1], a[1]]

for i in range(N):
    if c[0] == "@":
        print("\n    ", end=" ")
    else:
        print(c[0], end=" ")
    c = [c[1], c[2], D[tuple([c[1], c[2]])]]
