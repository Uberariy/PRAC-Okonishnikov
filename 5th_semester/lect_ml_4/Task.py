import numpy as np


class Preprocesser:
    
    def __init__(self):
        pass
    
    def fit(self, X, Y=None):
        pass
    
    def transform(self, X):
        pass
    
    def fit_transform(self, X, Y=None):
        pass
    
    
class MyOneHotEncoder(Preprocesser):
    
    def __init__(self, dtype=np.float64):
        super(Preprocesser).__init__()
        self.dtype = dtype
        
    def fit(self, X, Y=None):
        L = []
        for i in X:
            o = set()
            for j in X[i]:
                o.add(j)
            L += [sorted(list(o))]
        print(L)
        self.L = L
    
    def transform(self, X):
        res = []
        for i in range(X.shape[0]):
            P = []
            j0 = -1
            for j in X:
                j0 += 1
                o = [0]*len(self.L[j0])
                o[self.L[j0].index(X[j][i])] = 1
                P += [*o]
            res += [P]
        return(np.asarray(res))
    
    def fit_transform(self, X, Y=None):
        self.fit(X)
        return self.transform(X)
    
    def get_params(self, deep=True):
        return {"dtype": self.dtype}
    
    
class SimpleCounterEncoder:
    
    def __init__(self, dtype=np.float64):
        self.dtype=dtype
        
    def fit(self, X, Y):
        print(X.shape)
        L = np.array([[0.0]*X.shape[1]*3 for i in range(X.shape[0])])
        print(L)
        for stolb in enumerate(X):
            for j in enumerate(X[stolb[1]]):
                idx = [i for i in range(X.shape[0]) if X[stolb[1]][i] == j[1]]
                L[j[0]][stolb[0]*3] = sum(list(Y[idx]))/len(idx)
                L[j[0]][stolb[0]*3+1] = len(idx)/X.shape[0]
        self.L = L
            
    def transform(self, X, a=1e-5, b=1e-5):
        for stolb in range(X.shape[1]):
            for j in range(X.shape[0]):  
                self.L[j][stolb*3+2] = (self.L[j][stolb*3]+a)/(self.L[j][stolb*3+1]+b)
        return self.L
    
    def fit_transform(self, X, Y, a=1e-5, b=1e-5):
        self.fit(X, Y)
        return self.transform(X, a, b)
    
    def get_params(self, deep=True):
        return {"dtype": self.dtype}

    
def group_k_fold(size, n_splits=3, seed=1):
    idx = np.arange(size)
    np.random.seed(seed)
    idx = np.random.permutation(idx)
    n_ = size // n_splits
    for i in range(n_splits - 1):
        yield idx[i * n_ : (i + 1) * n_], np.hstack((idx[:i * n_], idx[(i + 1) * n_:]))
    yield idx[(n_splits - 1) * n_ :], idx[:(n_splits - 1) * n_]

    
class FoldCounters:
    
    def __init__(self, n_folds=3, dtype=np.float64):
        self.dtype = dtype
        self.n_folds = n_folds
        
    def fit(self, X, Y, seed=1):
        print(X.shape)
        gen = group_k_fold(X.shape[0], self.n_folds, seed)
        self.L = np.array([3*[0.0]*X.shape[1] for i in range(X.shape[0])])
        for idx_test, idx_train in gen:
            it = 0
            X_train = X.to_numpy()[idx_train]
            X_test = X.to_numpy()[idx_test]
            Y_train = np.array(Y[idx_train])
            np.sort(idx_test)
            print(X_train, X_test)
            for stolb in range(X_test.shape[1]):
                for j in range(X_test.shape[0]):
                    idx_total = X_train.shape[0]
                    idx_size = 0;   summ = 0
                    for k in range(X_train.shape[0]):
                        if X_train[k][stolb] == X_test[j][stolb]:
                            summ = summ + Y_train[k]
                            idx_size = idx_size + 1
                    self.L[idx_test[j]][it] = summ/idx_size
                    self.L[idx_test[j]][it+1] = idx_size/idx_total
                it += 3
            
    def transform(self, X, a=1e-5, b=1e-5):
        for stolb in range(X.shape[1]):
            for j in range(X.shape[0]):  
                self.L[j][stolb*3+2] = (self.L[j][stolb*3]+a)/(self.L[j][stolb*3+1]+b)
        print(self.L)
        return self.L
        
    def fit_transform(self, X, Y, a=1e-5, b=1e-5):
        self.fit(X, Y)
        return self.transform(X, a, b)
 
       
def weights(x, y):
    x1, numb = np.unique(x, return_counts=True)
    #vect = np.array([i for i in range(len(x1))])
    vect = np.zeros(len(x1))
    for stolb in range(len(x1)):
        idx = 0
        for j in range(x.shape[0]):
            if x[j] == x1[stolb] and y[j] > 0:
                idx += 1
        vect[stolb] = idx/numb[stolb]
    print(vect)
    return vect
