import sys
inp = sys.stdin.buffer.read()
res = inp.decode().encode("latin-1", errors='replace').decode('cp1251', errors='replace').encode('utf8')
sys.stdout.buffer.write(res)