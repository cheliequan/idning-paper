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

g : gen

gen: 
	#./scripts/protocol_gen.py
	./scripts/event_rpcgen.py common/protocol.rpc

c:
	scons -f scons.py -c

t: all
	scons -Q -f scons.py ning_target=test

#cscope
cs:
	echo "" > /tmp/cscope.files
	find /home/ning/idning-source/c/libevent-2.0.10-stable/  -name "*.h" >> /tmp/cscope.files
	find /home/ning/idning-source/c/libevent-2.0.10-stable/  -name "*.c" >> /tmp/cscope.files
	find . -name "*.c" >> /tmp/cscope.files
	find . -name "*.h" >> /tmp/cscope.files
	cscope  -i /tmp/cscope.files -R -b
	#
	ctags -L /tmp/cscope.files -R
	#######################################################################################
