def SUB(a, b):
	if type(a) == type(1):
		return type(a)(a-b)
	if type(a) == type(list()) or type(a) == type(tuple()):
		L = list();		d = list(b)
		for i in a:
			for j in d:
				if i == j:
					break
			else:
				L.append(i)
		return type(a)(L)
	
a, b = eval(input())
print(SUB(a, b))
