def lvlwhat(sttr, c):
	#print(sttr[1:len(sttr)-1], c)
	return c in sttr[1:len(sttr)-1]

sttr = str(input()).split()[0];	sttr = str(input()).split()[0]
H = len(sttr)-2
gas = 1 if lvlwhat(sttr, ".") and not lvlwhat(sttr, "~") and not lvlwhat(sttr, "#") else 0
while not lvlwhat(sttr, "~") and not lvlwhat(sttr, "#"):
	sttr = str(input()).split()[0]
	gas += 1 if lvlwhat(sttr, ".") and not lvlwhat(sttr, "~") and not lvlwhat(sttr, "#") else 0
water = 1 if lvlwhat(sttr, "~") else 0
while not lvlwhat(sttr, "#"):
	sttr = str(input()).split()[0]
	water += 1 if lvlwhat(sttr, "~") and not lvlwhat(sttr, "#") else 0
#print(gas, water)
L = water+gas
print("#"*(L+2))
for i in range(round(gas*H/L)):
	print("#"+"."*L+"#")
for i in range(round(water*H/L)):
	print("#"+"~"*L+"#")
print("#"*(L+2))
print("{:<"f"{max(gas*H, water*H)}""} {:>"f"{1+len(str(max(gas*H, water*H)))+len(str((water+gas)*H))}""}".format("."*gas*H,str(gas*H)+"/"+str((water+gas)*H)))
print("{:<"f"{max(gas*H, water*H)}""} {:>"f"{1+len(str(max(gas*H, water*H)))+len(str((water+gas)*H))}""}".format("~"*water*H,str(water*H)+"/"+str((water+gas)*H)))