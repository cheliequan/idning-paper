#!/usr/bin/scons -f
#coding:utf-8

#print BUILD_TARGETS
#print COMMAND_LINE_TARGETS 

import glob, os, subprocess, sys
common_src = glob.glob('common/*.c')
osd_src = glob.glob('osd/*.c')

test_c = glob.glob('test/*.c')
tests = [s.replace('.c', '') for s in test_c]
#tests = ['network_test', 'dlist_test', 'protocol_test']
print tests

def system(cmd):
    from subprocess import Popen, PIPE
    p = Popen(cmd, shell=True, bufsize = 102400, stdout=PIPE)
    return p.wait()

def test():
    test_out = glob.glob('test/*.out')
    for i in test_out:
        #print '#####', i
        if 0 == system(i):
            sys.stdout.write('.')
        else:
            print 'Test error on : ', i
            return -1
    print '\nThank God, All Tests Are Passed!!!!!!'


def compile():
    Library('common/common', common_src)

    for t in tests:
        target = '%s.out'%t
        src = '%s.c'%t
        #print target, src
        Program(target, [src], LIBS=['common'], LIBPATH=['common'], CPPPATH = ['common'], CCFLAGS='-Wall -D_DEBUG')

    Program( 'osd/osd.out', osd_src, 
        LIBS = ['common', 'event'], 
        LIBPATH = ['common'], 
        CPPPATH = ['common'], 
        CCFLAGS='-D_DEBUG')


target = ARGUMENTS.get('ning_target', 'compile') #default target is 'compile'!!! haha

eval(target+'()')

