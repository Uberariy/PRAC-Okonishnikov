import math
d = {}
count = 0
while True:
	count += 1
	l = input().split()
	if l[0][0] == ":":
		d[l[0][1::]] = l[1::]
	elif l[0] == "quit":
		print(eval(l[1].format(len(d)+1, count)))
		break
	else:
		if len(l[1::]) == 0:
			print(eval(d[l[0]][0], math.__dict__))
		else:
			print(eval(d[l[0]][-1], math.__dict__, {d[l[0]][i]: eval(l[i+1]) for i in range(len(l[1::]))}))
