N = int(input())
d = {}
c = ["."]
while True:
    s = input()
    if s.split() == []:
        break
    for i in s:
        if not i.isalpha():
            s = s.replace(i, ' ')
    c = s.split()
    for i in c:
        if N == len(i):
            d.setdefault(i.lower(), 0)
            d[i.lower()] += 1

print(" ".join([i for i in d if d[i]==d[max(d, key=d.get)]]))