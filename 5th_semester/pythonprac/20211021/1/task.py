l = list(input())
s = set()
for i in range(len(l)-1):
	if l[i].isalpha() and l[i+1].isalpha():
		s.add(str(l[i].lower())+str(l[i+1].lower()))
print(len(s))
		
