import sys
wav = sys.stdin.buffer.read()

import struct
try:
    markers = [*struct.unpack("<4s", wav[0:4]), *struct.unpack("<4s3s", wav[8:15]), *struct.unpack("<4s", wav[36:40])]
except (Exception, struct.error):
    print("NO")
else: 
    if markers == [b'RIFF', b'WAVE', b'fmt', b'data']:
        output = [*struct.unpack("<i", wav[4:8]), *struct.unpack("<hhi", wav[20:28]), *struct.unpack("<h", wav[34:36]), *struct.unpack("<i", wav[40:44])]
        print("Size={}, Type={}, Channels={}, Rate={}, Bits={}, Data size={}".format(*output))
    else:
        print("NO")
