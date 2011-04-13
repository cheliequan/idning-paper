#!/usr/bin/python 
#coding: utf8

import re,glob


def parse_file(path):
    #print path
    seq = []
    for line in open(path):
        if (line[0] == 'A'):
            seq.append(int(line.split()[5]))
    return seq

    
for count in [1,2,4,8]:
    path = '%dclients/*' % count

    for f in glob.glob(path):
        print parse_file(f)
    print '---------------------'
