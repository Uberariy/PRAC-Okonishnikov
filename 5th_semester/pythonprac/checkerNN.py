#!/usr/bin/env python3
'''
Homework checker.

Usage: chacker.py [task [test]]

task: program or directory
test: directory or file

If no or empty task and/or test is given, ask for it

Then run all tests against program and report the result
'''

import sys, os
from os.path import dirname, realpath, isfile, isdir, join, basename, getsize
from types import ModuleType
from typing import Any
from glob import glob
from subprocess import run, DEVNULL, PIPE, TimeoutExpired, CompletedProcess
from tempfile import NamedTemporaryFile as tempf, mkdtemp
from difflib import unified_diff
from collections import namedtuple
from shutil import rmtree
from functools import partial
import atexit
try:
    from pygments.lexers import DiffLexer
    from pygments.formatters import TerminalFormatter
    from pygments import highlight
except ModuleNotFoundError:
    highlight = DiffLexer = TerminalFormatter = None

GUIMODULE: ModuleType = None
GUIROOT: Any = None
PYFILES: tuple = (("Python files","*.py"),("all files","*.*")) # until 3.9
ALLFILES: tuple = (("all files","*.*"),) # until 3.9
PNAMES: list = ["prog", "main", "*"]
PTYPES: dict = {"Python": ".py", "Any": ""}
TDIRS: list = ["tests", "data", "."]
TINOUTS: list = [("in", "out"), ("dat", "res")]
TPATTS: list = ["[0-9]*.E", "E*.[0-9]*", "test*[0-9].E"]
RLIMIT_AS: int = 1024*1024*512
TMOUT: float = 1
MAXDIFF: int = 80

# XXX check if subprocess dies first
try:
    import resource
    resource.setrlimit(resource.RLIMIT_AS, (RLIMIT_AS, 3*RLIMIT_AS//2))
except:
    print("Warning: running with no memory limits", file=sys.stderr)

_ToDelete: str = mkdtemp()

def _todelete():
    if _ToDelete:
        rmtree(_ToDelete)

atexit.register(_todelete)

def guessprog(toProg: str) -> (str, str):
    """Guess a program to test

:param toProg:  File patern, directory name or empty string
:return:        Full program path, program type

:TODO:          more types (r. g. executables)
    """
    toProg = toProg or "."
    if isdir(toProg):
        d = realpath(toProg)
        for t, p in PTYPES.items():
            for n in PNAMES:
                if len(f:=list(filter(isfile, glob(join(d, n+p))))) > 0:
                    return f[0], t
    else: # toProg is pattern
        if len(f:=list(filter(isfile, glob(toProg)))) > 0:
            for t, p in PTYPES.items():
                if f[0].endswith(p):
                    return realpath(f[0]), t
    return "", ""

def guesstests(toTest: str) -> list: # list[tuple[str, str]]: # until 3.9
    """Guess tests pairs

:param toTest:  File name, directory name or empty string
:return:        List of (infile, outfile) full path pairs 
    """
    toTest = realpath(toTest or ".")
    F = [[]]
    if isdir(toTest):
        for d in TDIRS:
            D = realpath(join(toTest, d))
            for p in TPATTS:
                for ein, eout in TINOUTS:
                    P = join(D, p.replace("E", ein))
                    F.append([(f, g) for f in glob(P) if isfile(f) and isfile(g:=f.replace(ein, eout))])
    else:
        for ein, eout in TINOUTS:
            if ein in toTest:
                break
        F.append([(f, g) for f in glob(toTest) if isfile(f) and isfile(g:=f.replace(ein, eout))])
    
    return sorted(max(F, key=len))

def PythonRunner(Prog: str, In: str) -> str:
    """Run a program against input given
:param Prog:        Program to run
:param In:          Input data file name
:return:            Actual output file name
                    or Exception
"""
    with open(In) as fIn, tempf(prefix=basename(In)+".", dir=_ToDelete, delete=False) as fOut:
        try:
            res = run([sys.executable, Prog], stdin=fIn, stdout=fOut, stderr=PIPE, timeout=TMOUT)
        except TimeoutExpired as E:
            res = CompletedProcess(E.args, -1, stderr = str(E).encode(errors='replace'))

        # TODO syntax error
    if res.returncode:
        return RuntimeError(res.stderr.decode(errors='replace'))
    return fOut.name

def guessrunner(Prog: str, Ptype: str, In: str, Out:str) -> str:
    """Guess how to run program on certain test
:param Prog:        Program to run
:param Ptype:       Type of run program
:param In:          Input data filename
:param Out:         Canonical output data file name
:return:            Actual output file name
"""
    # TODO
    return PythonRunner

def LineChecker(Out:str, Cur:str) -> unified_diff:
    """Compare two text files, ignore leading/trailing whitespaces
:param Out: Canonical output
:param Cur: Actual output
:return:    unified_diff of files or None
"""
    outS, curS = getsize(Out), getsize(Cur)
    with open(Out) as fOut, open(Cur) as fCur:
        outT = [l.strip()+"\n" for l in fOut.readlines()]
        curT = [l.strip()+"\n" for l in fCur.readlines()]
    if outT == curT: return None
    if outS > MAXDIFF or curS > MAXDIFF:
        outT, curT = [f"Size differs: {outS}\n"], [f"Size differs: <output>\n"]
    return unified_diff(outT, curT, basename(Out), "<output>")

def guesschecker(Ptype: str, Out:str, Cur:str) -> str:
    """Guess how to compare canonical and actual outputs
:param Ptype:       Type of run program
:param Out:         Canonical output data filename
:param Cur:         Actual output data filename
:return:            Actual output file name
"""
    # TODO
    return LineChecker

def suite(Prog: str, Ptype: str, Tests: list) -> list:
    """Run all the tests against prog, collect results
:param Prog:    Program to test
:param Ptype:   Program type
:param Tests:   List or (input, output) filnames
:return:        List of results for each pair
    """
    Res = []
    for In, Out in Tests:
        Runner = guessrunner(Prog, Ptype, In, Out)
        Cur = Runner(Prog, In)
        if isinstance(Cur, Exception):
            Res.append((In, Cur))
        else:
            Checker = guesschecker(Ptype, In, Out)
            Res.append((In, Checker(Out, Cur)))
    return Res

def show(result: list) -> int:
    '''
    Show test results
:param result:  List of result objects
:return:        Error code
    '''
    # TODO gui show
    Error = 0
    for name, res in result:
        print(f"{basename(name)}: ", end="")
        if isinstance(res, Exception) or isinstance(res, str):
            print(f"{res}")
            Error |= 1
        elif hasattr(res, "send"):
            print(f"Output differs")
            if all((os.isatty(1), highlight, DiffLexer, TerminalFormatter, os.getenv("TERM"), os.getenv("TERM") != "dumb")):
                print(highlight("".join(res), DiffLexer(), TerminalFormatter()))
            else:
                sys.stdout.writelines(res)
            Error |= 2
        else:
            print("OK")
        print()
    return Error

def main(toProg: str, toTest: str) -> int:
    """Main workflow for manual run

:param toProg:  Test program pattern (needs guessing)
:param toTest:  Test directory pattern (needs guessing)
:return:        Total penalty (zero is ideal, negative is system malfunction)
    """
    (Prog, Ptype), Tests = guessprog(toProg), guesstests(toTest)
    #print(f"{Prog=}, {Ptype=}, {Tests=}")
    res = suite(Prog, Ptype, Tests)
    return show(res)

def _init_GUI() -> bool:
    """Start Tk, hide root window"""
    global GUIMODULE, GUIROOT
    try:
        import tkinter.filedialog as GUIMODULE
        from tkinter import Tk
        from tkinter.simpledialog import askstring
        GUIROOT = Tk()
        GUIMODULE.askstring = partial(askstring, GUIROOT)
        GUIROOT.withdraw()
    except Exception as E:
        return False
    return True

def askprog(prompt: str = "File name", pattern: tuple = ALLFILES) -> str:
    """Ask for file name

:param prompt:  Actual file purpose
:param pattern: File patterns in tkinter chooser format
:return:        File name or "" if none chosen
"""
    from tkinter.filedialog import askopenfilename
    _init_GUI()

    res = GUIMODULE.askopenfilename(title = prompt, filetypes = pattern, initialdir = dirname(sys.argv[0]))
    if res == ():
        res = GUIMODULE.askstring("Enter HitHub/GitLab file URL:")

    return res if type(res) is str else ""

def askdir(prompt: str = "Directory name") -> str:
    """Ask for directory name

:param prompt:  Actual directory purpose
:return:        Directory name or "" if none chosen
"""
    from tkinter.filedialog import askdirectory
    _init_GUI()

    res = GUIMODULE.askdirectory(title = prompt, initialdir = dirname(sys.argv[0]))
    if res == ():
        res = GUIMODULE.askstring("Enter HitHub/GitLab directory URL:")
    return res if type(res) is str else ""

import urllib.request
import zipfile
import io
def gitlabdir(url: str) -> str:
    """Download directory from GitLab public archive
:param url: Directory URL (like https://git.cs.msu.ru/gkuryachiy/prac/-/tree/master/checker/tests)
:return:    Temporary directory with files
"""
    # https://git.cs.msu.ru/s02180538/pythonprac/-/tree/master/20201112_3/data
    # https://git.cs.msu.ru/s02180538/pythonprac/-/archive/master/arc.zip?path=20201112_3/data
    base, path = url.split("/-/")
    branch = path.split("/")[1]
    path = "/".join(path.split("/")[2:])
    zipurl = f"{base}/-/archive/{branch}/archive.zip?path={path}"
    with urllib.request.urlopen(zipurl) as f:
        zipdir = zipfile.ZipFile(io.BytesIO(f.read()))
    zipdir.extractall(_ToDelete)
    return join(_ToDelete, dirname([e for e in zipdir.infolist() if not e.is_dir()][0].filename))

def githubdir(url: str) -> str:
    """Download directory from GitHub public archive
:param url: Directory ORL (like https://github.com/FrBrGeorge/LinuxDev2020/tree/master/03_TerminalProject)
:return:    Temporary directory with files
"""
    # https://github.com/TheGhost8/pythonprac/tree/master/20201112_3/data
    # https://codeload.github.com/TheGhost8/pythonprac/zip/master
    base, path = url.split("/tree/")
    name = base.split("/")[-1]
    branch = path.split("/")[0]
    path = f"{name}-{branch}/" + "/".join(path.split("/")[1:])
    base = base.replace("//","//codeload.")
    zipurl = f"{base}/zip/{branch}"
    with urllib.request.urlopen(zipurl) as f:
        zipdir = zipfile.ZipFile(io.BytesIO(f.read()))
    files = [n for n in zipdir.namelist() if n.startswith(path)]
    zipdir.extractall(_ToDelete, files)
    return join(_ToDelete, path)
    #return join(_ToDelete, dirname([e for e in zipdir.infolist() if not e.is_dir()][0].filename))

def gitlabfile(url: str) -> str:
    """
    Download single file from GitLab
:param url: File URL
:return:    Temporary file name
    """
    # https://git.cs.msu.ru/gkuryachiy/prac/-/blob/master/checker/README.md
    # https://git.cs.msu.ru/gkuryachiy/prac/-/raw/master/checker/README.md?inline=false
    base, path = url.split("/-/")
    dpath = path.replace("blob/","raw/")+"?inline=false"
    outf = tempf(suffix="."+basename(path), dir=_ToDelete, delete=False)
    with urllib.request.urlopen(base+"/-/"+dpath) as f:
        outf.write(f.read())
    outf.close()
    return outf.name

def githubfile(url: str) -> str:
    """
    Download single file from GitHub
:param url: File URL
:return:    Temporary file name
    """
    # https://github.com/cmc-python-dev/python2020/blob/master/README.md
    # https://raw.githubusercontent.com/cmc-python-dev/python2020/master/README.md
    durl = url.replace("/blob/","/").replace("/github.com/", "/raw.githubusercontent.com/")
    print("##", durl)
    outf = tempf(suffix="."+basename(url), dir=_ToDelete, delete=False)
    with urllib.request.urlopen(durl) as f:
        outf.write(f.read())
    outf.close()
    return outf.name
    sys.exit(res)

if __name__ == "__main__":
    # TODO help
    if len(sys.argv) < 2:
        toProg = askprog("Program to test", PYFILES)
        toTest = askdir("Tests directory")
    else:
        toProg = sys.argv[1]
        toTest = sys.argv[2] if len(sys.argv) > 2 else dirname(toProg)
    if ":/" in toProg:
        if not("://" in toProg):
            toProg = toProg.replace(":/", "://")
        if "/-/" in toProg:
            toProg = gitlabfile(toProg)
        elif "//github" in toProg:
            toProg = githubfile(toProg)
    if ":/" in toTest:
        if not("://" in toTest):
            toTest = toTest.replace(":/", "://")
        if "/-/" in toTest:
            toTest = gitlabdir(toTest)
        elif "//github" in toTest:
            toTest = githubdir(toTest)
    res = main(toProg, toTest)
    if len(sys.argv) > 3:
        print(f"Temporary directory: {_ToDelete}")
        input("Press any key ")