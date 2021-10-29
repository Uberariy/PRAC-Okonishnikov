def distance(a, b):
    return ((float(a[0])-float(b[0]))**2+(float(a[1])-float(b[1]))**2+(float(a[2])-float(b[2]))**2)**0.5

#import time
#tick = time.perf_counter()
a = input().split(" ")
l = []
while a[0] != ".":
    l.append(tuple(a))
    a = input().split(" ")

max = 0
t = [0,0]
n = len(l)
for i in range(n):
    for j in range(i,n):
        if distance(l[i], l[j])>max:
            max = distance(l[i], l[j])
            t[0], t[1] = l[i][3], l[j][3]
    
t.sort()
print(t[0], t[1])
#print(time.perf_counter()-tick)