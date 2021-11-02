def Pareto(*l):
	M = []
	# [(x,y) for x,y in l for a,b in l if all([x>=a and y>=b and (x>a or y>b) for a, b in l])
	for ab in l:
		a = 1
		for xy in l:
			if xy == ab:
				continue
			if xy[0] >= ab[0] and xy[1] >= ab[1] and (xy[0] > ab[0] or xy[1] > ab[1]):
				a = 0
				continue
		if a == 1:
			M.append(ab)
	return(tuple(M))

print(Pareto(*(eval(input()))))

