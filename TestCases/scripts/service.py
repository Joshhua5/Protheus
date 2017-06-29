#!/usr/bin/python3
import time
import os

def main():
    root = os.path.dirname(os.path.realpath(__file__))
    os.chdir(root)
    while(True):
        os.popen("./start.py")
        time.sleep(60 * 60 * 12)

main()
