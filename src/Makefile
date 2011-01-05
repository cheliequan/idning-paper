#usage :
# alias m='make'
# m
# m c
# m t 
# m cs
#
#

all: cs
	scons -Q -f scons.py

c:
	scons -f scons.py -c

t: all
	scons -Q -f scons.py ning_target=test


#cscope
cs:
	cscope -R -b
	ctags -R
