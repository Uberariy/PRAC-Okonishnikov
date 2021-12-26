a = int(input())
if a%2==0 and a%25==0:
    print("A + ", end = "")
else:
    print("A - ", end = "")
if a%2==1 and a%25==0:
    print("B + ", end = "")
else:
    print("B - ", end = "")
if a%8==0:
    print("C +", end = "")
else:
    print("C -", end = "")