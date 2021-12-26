def SUB(a, b):
	try:
		c = type(a)(a-b)
		return c
	except Exception:
		L = list();		d = list(b)
		for i in a:
			for j in d:
				if i == j:
					break
			else:
				L.append(i)
		return type(a)(L)
	
#import sys
#exec(sys.stdin.read())
a, b = eval(input())
print(SUB(a, b))
