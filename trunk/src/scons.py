#!/usr/bin/scons -f
#coding:utf-8

import glob, os
common = glob.glob('common/*.c')
#print common

tests = ['network_test']
for t in tests:
    target = 'test/%s.out'%t
    src = 'test/%s.c'%t
    print target, src
    Program(target, common + [src], CPPPATH = ['common'])

#test_src = common + ['test/network_test.c']


#Program(
    #target = 'test/test.out', 
    #source = test_src, 
    ##LIBS = ['lib1', 'lib2'], 
    ##LIBPATH = ['lib1/lib', 'lib2/lib'], 
    #CPPPATH = ['common'], 
    #CCFLAGS='-D_DEBUG')
