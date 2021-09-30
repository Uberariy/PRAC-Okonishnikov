def longestCommonPrefix(x):
    y = x;
    for i in range(len(y)):
        y[i] = x[i].lstrip();
        print(y[i]);
    l = min(y, key=len);
    a = "";
    for j in range(0,len(l),1):
        b = l[j];
        for i in range(len(y)):
            if (y[i][j] != b):
                return a;
        a += b;
    return a

if __name__ == "__main__":
    print(longestCommonPrefix(['123', '1 2', '1 23']))

