C = {}
for w in input().split():
	C[w] = C.setdefault(w, 0) + 1
print(C)
