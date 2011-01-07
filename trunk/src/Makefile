#usage :
# alias m='make'
# m
# m c
# m t 
# m cs
#
#

all: protocol_gen cs
	scons -Q -f scons.py

protocol_gen: 
	./scripts/protocol_gen.py

c:
	scons -f scons.py -c

t: all
	scons -Q -f scons.py ning_target=test


#cscope
cs:
	cscope -I/usr/include -R -b
	ctags -R
