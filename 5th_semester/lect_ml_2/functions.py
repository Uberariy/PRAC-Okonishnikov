from typing import List


def prod_non_zero_diag(X: List[List[int]]) -> int:
    N = min(len(X), len(X[0]))
    a = -1;
    for i in range(N):
        if 0 != X[i][i]:
            a = 1
            for j in range(N):
                if 0 != X[j][j]:
                    a = a*X[j][j]
            break
    return a


def are_multisets_equal(x: List[int], y: List[int]) -> bool:
    b = x
    for i in range(len(y)):
        for j in range(0, len(b)):
            if y[i]==b[j]:
                b.pop(j)
                break
    return b == []


def max_after_zero(x: List[int]) -> int:
    a = x
    s = []
    while True:
        if 0 in a and a.index(0)+1 < len(a):
            s += [a[a.index(0)+1]]
            a = a[a.index(0)+1:len(a)]
        elif s == []:
            return -1
        else:
            return max(s)


def convert_image(image: List[List[List[float]]], weights: List[float]) -> List[List[float]]:
    P = [[0 for i in range(len(image))]for j in range(len(image[0]))]
    for i in range(len(image)):
        for j in range(len(image[0])):
            for h in range(len(weights)):
                P[i][j] += image[i][j][h]*weights[h]
    return(P)

def run_length_encoding(x: List[int]) -> (List[int], List[int]):
    i = 0;  o = [];  p = []
    while i<len(x):
        j = i+1
        while j<len(x) and x[i] == x[j]:
            j += 1
        o += [x[i]]
        p += [j-i]
        i = j
    return (o, p)


def pairwise_distance(X: List[List[float]], Y: List[List[float]]) -> List[List[float]]:
    P = [[0 for i in range(len(X))]for j in range(len(Y))]
    for i in range(len(X)):
        for j in range(len(Y)):
            for h in range(len(X[0])):
                P[i][j] += (X[i][h]-Y[j][h])**2
            P[i][j] = P[i][j]**(0.5)
            #if len(Y)<=10:
            #    print(P[i][j])
    return(P)
