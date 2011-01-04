#!/usr/bin/scons -f
#coding:utf-8

Program(target = 'test/test.out', 
    source = ['test/network_test.c'], 
    #LIBS = ['lib1', 'lib2'], 
    #LIBPATH = ['lib1/lib', 'lib2/lib'], 
    CPPPATH = ['common'], 
    CCFLAGS='-D_DEBUG')
