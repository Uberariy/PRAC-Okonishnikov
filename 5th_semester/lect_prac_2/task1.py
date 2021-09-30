x,y,r = eval(input())
a,b = eval(input())
while a != 0 or b != 0:
    if (a-x)**2+(b-y)**2>r**2:
        print("NO")
        break
    a,b = eval(input())
else:
    print("YES")
