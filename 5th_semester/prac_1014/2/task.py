from math import *

func = lambda x: eval(sstr[4]) 
def fun(x):
	return func(x)

def getmax(func, A, B, W):
	max = fun(A)
	for i in range(0, W+1):
		max = func(A+i*(B-A)/W) if func(A+i*(B-A)/W) > max else max
	return max

def getmin(func, A, B, W):
	min = fun(A)
	for i in range(0, W+1):
		min = func(A+i*(B-A)/W) if func(A+i*(B-A)/W) < min else min
	return min

sstr = list(str(input()).split(" "))
W, H, A, B = int(sstr[0]), int(sstr[1]), int(sstr[2]), int(sstr[3])
C, D = getmin(fun, A, B, W), getmax(fun, A, B, W)
C -= (D-C)/H
xstep = (B-A)/W
ystep = (D-C)/H
#print(A, B, C, D, xstep, ystep)
y = D
while y>C:
	x = A
	while x<B:
		if abs(fun(x)-y) < 0.5*((abs(D-C))/(H)+ystep*(abs(B-A))/(W)):
			print("*", end="")
		else:
			print(" ", end="")
		x += xstep
	print("")
	y -= ystep
