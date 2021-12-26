s = list(input())
gl = {'a', 'e', 'o', 'u', 'i', 'y', "A", "O", "E", "I", "U", "Y"}
sogl = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") - gl
ngl = 0
nsogl = 0
for i in s:
	if i in gl:
		ngl += 1
	elif i in sogl:
		nsogl += 1
print(ngl, nsogl)
		
