import numpy as np


def prod_non_zero_diag(X: np.ndarray) -> int:
    a = np.diagonal(X)
    a = a[a != 0]
    if a.size == 0:
        return -1
    return np.prod(a)


def are_multisets_equal(x: np.ndarray, y: np.ndarray) -> bool:
    return list(np.sort(x)) == list(np.sort(y))


def max_after_zero(x: np.ndarray) -> int:
    ind = np.where(x == 0)
    if ind[0].size == 0:
        return -1
    m = 0
    for i in ind[0]:
        if i+1 < len(x) and x[i+1] > m:
            m = x[i+1]
    return -1 if m == 0 else m


def convert_image(image: np.ndarray, weights: np.ndarray) -> np.ndarray:
    #return np.apply_along_axis(np.dot, 2, image, weights)
    #n = image.shape[0]
    #return np.array([[np.dot(image[j][i], weights) for i in range(n)] for j in range(n)])
    return np.dot(image,weights)


def run_length_encoding(x: np.ndarray) -> (np.ndarray, np.ndarray):
    if x[len(x)-1]!=x[len(x)-2]:
        indexes = list(np.nonzero(np.r_[1, np.diff(x)[:-1], len(x)])[0])
    else:
        indexes = list(np.nonzero(np.r_[1, np.diff(x)[:-1]])[0])
    tmp = indexes[1::] + [len(x)]
    u = np.array(x[indexes])
    return(u, np.subtract(tmp, indexes))


def pairwise_distance(X: np.ndarray, Y: np.ndarray) -> np.ndarray:
    #return np.apply_along_axis(np.dot, 2, X[:, np.newaxis] - Y, X[:, np.newaxis] - Y)
    #print(X.shape, np.linalg.norm(X[:, np.newaxis]-Y, axis=2))
    return np.linalg.norm(X[:, np.newaxis]-Y, axis=2)
    #return np.array([[float(sum((X[j]-Y[i])**2)) for i in range(n)] for j in range(n)])**0.5
