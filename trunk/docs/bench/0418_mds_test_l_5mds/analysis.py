#!/usr/bin/python 
#coding: utf8

import re,glob
machines = ['10000', '10001', '10002', '3', '4']

def parse_file(path):
    data = file(path).read()
    cluster_maps = data.split('\n------------\n')
    for c in cluster_maps:
        #print c
        kv = {'10000': '', '10001': '', '10002': '', '3': '', '4' : ''}
        for m in re.findall(r'\( *(\d*)\) LOAD: (\d*)', c, re.DOTALL):
            kv[m[0]] = m[1];
        print '%s\t%s\t%s\t%s\t%s'%( kv['10000'], kv['10001'], kv['10002'], kv['3'], kv['4'])




parse_file('log/cmgr.cfg.log')
    
