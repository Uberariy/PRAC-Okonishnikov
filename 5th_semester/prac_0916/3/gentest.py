import random
a = [int(input())]
for i in range(9):
    a.append(random.randint(1,100))
random.shuffle(a)
print(a)