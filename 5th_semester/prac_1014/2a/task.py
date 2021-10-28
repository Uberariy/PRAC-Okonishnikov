from math import *
xA, xB, yA, yB = -20, 20, -1, 1
xstep = xB-xA/80
ystep = yB-yA/25
x = xA
while x<=xB:
	print(" + ", end="")
	y = yA
	while y<=yB:
		if (sin(x)-y)**2 < 0.003:
			print("#", end="")
		else:
			print(".", end="")
		y += (yB-yA)/80
	if abs(x//1-x)<(xB-xA)/200:
		print(" y =", round(x))
	else:
		print(" + ")
	x += (xB-xA)/200
