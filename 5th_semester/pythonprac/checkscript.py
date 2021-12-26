"Testing script for pythonprac (2021y)"
"""
1. Clone pythonprac in ~/; 
2. Put script in ~/pythonprac; 
3. Put checkerNN.py in ~/pythonprac;
4. Execute!
"""

import os

upgradedir = []
readydir = ['20211028', '20210916', '20211014', '20211111', '20210923', '20211118', '20211021', '20210930', '20211213', '20211125']

for i in os.listdir('.'):
    if str(i)[:4] == "2021":# and str(i)[:8] not in readydir and str(i)[:8] not in upgradedir:
        for j in range(1, 5):
            print("dir: "+i+", program:", j)
            # Following two lines process execution (commend them, if u want to avoid it)
            for k in range(1, 4):
                os.system("python3 ~/pythonprac/"+i+"/"+str(j)+"/task.py < ~/dimast/"+i+"/"+str(j)+"/tests/"+str(k)+".in > ~/pythonprac/"+i+"/"+str(j)+"/tests/"+str(k)+".out;")
            # Following line process testing (commend it, if u want to avoid testing)
            if str(i)[:8] == '20211014' and j == 2:
                continue
            os.system("python3 checkerNN.py ~/pythonprac/"+i+"/"+str(j)+"/task.py ~/dimast/"+i+"/"+str(j)+"/tests")