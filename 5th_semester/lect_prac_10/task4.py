from collections import defaultdict
mutex = defaultdict(list)
class Lock:

    def __get__(self, obj, cls):
        for i in mutex.items():
            if i[0] != id(obj) and i[1][1] == 1 and i[1][0] == mutex[id(obj)][0]:
                return None
        if len(mutex[id(obj)]) >= 2:
            mutex[id(obj)][1] = 1
            return mutex[id(obj)][0]

    def __set__(self, obj, val):
        mutex[id(obj)] = [val, 0]

    def __delete__(self, obj):
        if len(mutex[id(obj)]) >= 2:
            mutex[id(obj)][1] = 0
        pass

    @staticmethod
    def locked(cls):
        class Deleter(cls):
            lock = Lock()

            def __del__(self):
                del self.lock
                if getattr(super(), "__del__", None) is not None:
                    super().__del__()

        return Deleter

import sys
exec(sys.stdin.read())
