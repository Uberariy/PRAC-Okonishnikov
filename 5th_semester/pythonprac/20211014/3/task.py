import math as math
def lvlwhat(sttr, c):
	return c in sttr[1:len(sttr)-1]

def count(sttr, c):
	cou = 0
	for i in sttr[1:len(sttr)-1]:
		cou = cou + 1 if i == c else cou
	return cou

sttr = str(input()).split()[0];	sttr = str(input()).split()[0]
H = len(sttr)-2
L = gas = water = 0
while not lvlwhat(sttr, "#"):
	gas += count(sttr, ".")
	water += count(sttr, "~")
	L += 1
	sttr = str(input()).split()[0]

print("#"*(L+2))
for i in range(math.trunc(gas/L)):
	print("#"+"."*L+"#")
for i in range(math.ceil(water/L)):
	print("#"+"~"*L+"#")
print("#"*(L+2))
o = int(max(round(gas/max(gas,water)*20), water/max(gas,water)*20))
print("{:<"f"{o}""} {:>"f"{1+len(str(max(gas, water)))+len(str((water+gas)))}""}".format("."*round(gas/max(gas,water)*20),str(gas)+"/"+str((water+gas))))
print("{:<"f"{o}""} {:>"f"{1+len(str(max(gas, water)))+len(str((water+gas)))}""}".format("~"*round(water/max(gas,water)*20),str(water)+"/"+str((water+gas))))