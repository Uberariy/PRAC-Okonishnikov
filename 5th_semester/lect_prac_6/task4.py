import sys
N = int(input())
Txt = sys.stdin.read().replace("\n    ", " @ ").split()
s = set(tuple([Txt[i], Txt[i+1], Txt[i+2]]) for i in range(len(Txt)-2))
#print(s)
for c in s:
    break
for i in range(N-2):
    for j in s:
        if c[1] == j[0] and c[2] == j[1]:
            if c[0] == "@":
                print("\n    ", end=" ")
            else:
                print(c[0], end=" ")
            c = j
