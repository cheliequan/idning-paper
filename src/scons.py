#!/usr/bin/scons -f
#coding:utf-8

import glob, os
common = glob.glob('common/*.c')
#print common

tests = ['network_test']

Library('common/common', common)

for t in tests:
    target = 'test/%s.out'%t
    src = 'test/%s.c'%t
    print target, src
    Program(target, [src], LIBS=['common'], LIBPATH=['common'], CPPPATH = ['common'], CCFLAGS='-D_DEBUG')

#test_src = common + ['test/network_test.c']

osd_src = glob.glob('osd/*.c')

Program(
    'osd/osd.out', 
    osd_src, 
    LIBS = ['common'], 
    LIBPATH = ['common'], 
    CPPPATH = ['common'], 
    CCFLAGS='-D_DEBUG')
