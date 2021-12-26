"Script making urls for pythonprac (2021y)"
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
            if str(i)[:8] == '20211014' and j == 2:
                continue
            print("dir: "+i+", program with url:", j)
            os.system("touch ~/pythonprac/"+i+"/"+str(j)+"/urls")
            os.system("echo https://git.cs.msu.ru/s02190234/pythonprac2021/-/tree/main/"+i+"/"+str(j)+"/tests > ~/pythonprac/"+i+"/"+str(j)+"/urls")
            os.system("echo https://git.cs.msu.ru/s02190141/pythonprac-2021/-/tree/main/"+i+"/"+str(j)+"/tests >> ~/pythonprac/"+i+"/"+str(j)+"/urls")
            os.system("echo https://github.com/stamplevskiyd/pythonprac-2021/tree/main/"+i+"/"+str(j)+"/tests >> ~/pythonprac/"+i+"/"+str(j)+"/urls")