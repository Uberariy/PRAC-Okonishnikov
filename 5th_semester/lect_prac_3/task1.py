a = list(eval(input()))
for i in range(len(a)):
    a[i] = list(a[i])
a = sorted(a)

j = 0
while j < len(a):
    i = j+1
    while i < len(a) and a[i][0] < a[j][1]:
        a[j][0] = min(a[j][0], a[i][0])
        a[j][1] = max(a[j][1], a[i][1])
        a.pop(i)
    j += 1

sum = 0
for k in a:
    sum += k[1]-k[0]
print(sum)