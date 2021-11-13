import numpy as np

class MinMaxScaler:
    mins, dist = np.asarray([]), np.asarray([])
    def fit(self, data):
        self.mins = np.asarray([min(i) for i in data.T])
        self.dist = np.asarray([max(i)-min(i) for i in data.T])
        pass
        
    def transform(self, data):
        return ((data - self.mins) / self.dist)


class StandardScaler:
    aver, disp = np.asarray([]), np.asarray([])
    def fit(self, data):
        self.aver = np.asarray([sum(i)/len(i) for i in data.T])
        self.disp = np.asarray([sum([(j-sum(i)/len(i))**2 for j in i])/len(i) for i in data.T])
        
    def transform(self, data):
        return ((data - self.aver) / self.disp**0.5)
