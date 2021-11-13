import numpy as np
from collections import defaultdict

def kfold_split(num_objects, num_folds):
    ans = []
    fold_len = num_objects // num_folds
    for i in range(num_folds-1):
        ans.append((np.asarray([j for j in range(num_objects) if j not in range(fold_len*i, fold_len*(i+1))]), np.asarray([k for k in range(fold_len*i, fold_len*(i+1))])))
    ans.append((np.asarray([j for j in range(fold_len*(i+1))]), np.asarray([j for j in range(fold_len*(i+1), num_objects)])))
    return ans

def knn_cv_score(X, y, parameters, score_function, folds, knn_class):
    D = dict()
    for l in parameters['normalizers']:
        for i in parameters['n_neighbors']:
            for j in parameters['metrics']:
                for k in parameters['weights']:

                    X_scaled = X # if scaler is None
                    if l[0] is not None:
                        l[0].fit(X)
                        X_scaled = l[0].transform(X)
                    
                    clf = knn_class(n_neighbors=i, weights=k, metric=j)
                    tmp = []
                    for train_idx, test_idx in folds:
                        X_train, X_test = X_scaled[train_idx], X_scaled[test_idx]
                        y_train, y_test = y[train_idx], y[test_idx]

                        clf.fit(X_train, y_train)
                        Z = clf.predict(X_test)
                        tmp.append(score_function(y_test, Z))

                    D[(l[1], i, j, k)] = np.mean(tmp)
    return D
 