import sys
inp = sys.stdin.buffer.read()
res = inp.decode('utf8').encode('cp1251').decode("latin-1").encode()
sys.stdout.buffer.write(res)