class WeAre():
    count = 0

    def __init__(self):
        WeAre.count = WeAre.count+1
    def __del__(self):
        WeAre.count = WeAre.count-1

import sys
exec(sys.stdin.read())