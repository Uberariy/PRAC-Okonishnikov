import numpy as np
from sklearn.svm import SVC


def train_svm_and_predict(train_features, train_target, test_features):
    from sklearn.preprocessing import StandardScaler
    std = StandardScaler()
    X_train_std = std.fit_transform(train_features)
    X_test_std = std.transform(test_features)

    svc_m = SVC(kernel='poly', C=2.242, degree=2, gamma='scale')
    svc_m.fit(X_train_std, train_target)
    Z = svc_m.predict(X_test_std)
    return Z
