#!/usr/bin/python
import sys, string
prevline = " ";
flag = 0;
for line in sys.stdin:
    if prevline ==" " :
        prevline = line
        continue
    elif line == prevline:
          if flag == 0:
             line3 = line.split()
             print('<'+line3[0]+', '+line3[1]+', '+line3[2]+'>')
             print('<'+line3[1]+', '+line3[0]+', '+line3[2]+'>')
             print('<'+line3[2]+', '+line3[1]+', '+line3[0]+'>')
             flag = 1
             prevline = line
          else:
             prevline = line
    else:
          flag = 0;
          prevline = line;
                
