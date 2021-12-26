import os
import sys
import zipfile

arch = sys.stdin.read()

f = open("tmp.txt", "wb")
f.write(bytearray.fromhex(arch))
f.close()

zipu = zipfile.ZipFile("tmp.txt")
zipdir = [i for i in zipu.filelist if not i.is_dir()]
print(len(zipdir), sum([i.file_size for i in zipdir]))

os.remove("tmp.txt")
