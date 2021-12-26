a, b = eval(input())
print([i for i in range(a, b, 1) if all([i%j for j in range(2,i)]) and i!=1])