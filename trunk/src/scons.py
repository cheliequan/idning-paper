#!/usr/bin/scons -f
#coding:utf-8

#print BUILD_TARGETS
#print COMMAND_LINE_TARGETS 

import glob, os
common_src = glob.glob('common/*.c')
osd_src = glob.glob('osd/*.c')

tests = ['network_test', 'dlist_test']

def test():
    test_out = glob.glob('test/*.out')
    for i in test_out:
        print '.....', i
        os.system(i)

def compile():
    Library('common/common', common_src)

    for t in tests:
        target = 'test/%s.out'%t
        src = 'test/%s.c'%t
        #print target, src
        Program(target, [src], LIBS=['common'], LIBPATH=['common'], CPPPATH = ['common'], CCFLAGS='-D_DEBUG')

    Program( 'osd/osd.out', osd_src, 
        LIBS = ['common', 'event'], 
        LIBPATH = ['common'], 
        CPPPATH = ['common'], 
        CCFLAGS='-D_DEBUG')


target = ARGUMENTS.get('ning_target', 'compile') #default target is 'compile'!!! haha

eval(target+'()')

