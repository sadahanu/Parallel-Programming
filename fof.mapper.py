#!/usr/bin/python
# Sadahanu 2014
import sys, string
for line in sys.stdin:
    frlist = string.split(line)
    if len(frlist)<3:
        break
    for i in range (1,len(frlist)-1):
        for j in range(i+1, len(frlist)):
            #tri = [int(frlist[0]),int(frlist[i]),int(frlist[j])]
            tri = [frlist[0],frlist[i],frlist[j]]
            stri = sorted(tri)
            print(' '.join(stri))
