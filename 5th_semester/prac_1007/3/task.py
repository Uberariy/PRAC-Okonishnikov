def Bisect(c, s):
	if len(s) < 1:
		return False
	if len(s) == 1:
		return s[0] == c
	if c == s[len(s)//2]:
		return True
	elif c < s[len(s)//2]:
		return Bisect(c, s[0:len(s)//2:])
	elif c > s[len(s)//2]:
		return Bisect(c, s[len(s)//2:len(s):])
	
a,*b = eval(input())
b = b[0]
print(Bisect(a, b))
