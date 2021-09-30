def check(l, file1):
    from collections import Counter
    s = l.casefold()
    d = s.split()
    d = sorted(d)
    cnt = Counter(d)
    f = open(file1, 'w')
    for k, v in cnt.items():
        f.write(k)
        f.write(" ")
        f.write(str(v))
        f.write("\n")
    f.close()
    return "a"

if __name__ == "__main__":
    check("a aa abC aa ac abc bcd a", "file.txt")

