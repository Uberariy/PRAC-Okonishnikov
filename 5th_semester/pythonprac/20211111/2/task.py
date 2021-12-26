class BadTriangle(Exception):
	pass
class InvalidInput(Exception):
	pass
from typing import SupportsFloat
	
# Я считаю, что bool - это числа, поэтому для них проходить должно
def TriangleSquare(s):
	if type((1, 2))!=type(s) or len(s)!=3:
		raise(InvalidInput)
	x,y,z = s
	for coord in [x[0], x[1], y[0], y[1], z[0], z[1]]:  #int или bool то приведутся к float, а complex/str/...-нет
		if not isinstance(coord, SupportsFloat):
			raise(BadTriangle)
	a = ((x[0]-y[0])**2 + (x[1]-y[1])**2)**0.5
	b = ((y[0]-z[0])**2 + (y[1]-z[1])**2)**0.5
	c = ((z[0]-x[0])**2 + (z[1]-x[1])**2)**0.5
	if max(a,b,c) >= a+b+c-max(a,b,c) or a<=0 or b<=0 or c<=0:
		raise(BadTriangle)
	else:
		p = (a+b+c)/2
		S = (p*(p-a)*(p-b)*(p-c))**0.5
		if S==0:
			raise(BadTriangle)
		print(f"{S:.{2}f}")
		return True
	
	pass

while True:
	try:
		s = eval(input())
		if TriangleSquare(s):
			break
	except BadTriangle as E:
		print("Not a triangle")
	except (InvalidInput, Exception) as E:
		print("Invalid input")	

