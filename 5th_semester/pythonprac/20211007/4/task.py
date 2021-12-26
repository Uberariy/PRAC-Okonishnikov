from math import *
def Calc(s, t ,u):
	a = lambda x: eval(s)
	b = lambda x: eval(t)
	c = lambda x, y: eval(u)
	def fun(x):
		return(c(a(x), b(x)))
	return fun
	
#import sys
#exec(sys.stdin.read())
F = Calc(*eval(input()))
print(F(eval(input())))
