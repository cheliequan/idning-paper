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
	find . -name '*.gcov' | xargs rm -f
	find . -name '*.gcda' | xargs rm -f
	find . -name '*.gcno' | xargs rm -f
	#find . -name '*.out' | xargs rm -f
	find . -name '*.o' | xargs rm -f

t: all
	scons -Q -f scons.py ning_target=test

#cscope
cs:
	echo "" > /tmp/cscope.files
	find /home/ning/idning-source/c/libevent-2.0.10-stable/  -name "*.h" >> /tmp/cscope.files
	find /home/ning/idning-source/c/libevent-2.0.10-stable/  -name "*.c" >> /tmp/cscope.files
	find /home/ning/idning-source/c/fuse-2.8.5 -name "*.c" >> /tmp/cscope.files
	find /home/ning/idning-source/c/fuse-2.8.5 -name "*.h" >> /tmp/cscope.files
	#
	find . -name "*.c" >> /tmp/cscope.files
	find . -name "*.h" >> /tmp/cscope.files
	cscope  -i /tmp/cscope.files -I/usr/include/ -R -b
	#
	ctags -L /tmp/cscope.files -R
	#######################################################################################

gcov:
	# -o 指的是path. 垃圾.
	gcov client/mount.c -o client
	gcov client/osd_conn.c -o client
	gcov client/mds_conn.c -o client

umount:
	-sudo umount /tmp/a -l
m:
	-sudo umount /tmp/a -l
	./client/mount.out /tmp/a -d 

ARG=--window-with-profile=tmp --working-directory=/home/ning/idning-paper/src --hide-menubar

# show log windows
logo:
	gnome-terminal $(ARG) -e "tail -f log/cmgr.cfg.log" 	-t "cmgr" 	--geometry=80x22+0+0
	gnome-terminal $(ARG) -e "tail -f log/client.cfg.log" 	-t "client" --geometry=80x22+600+0
	gnome-terminal $(ARG) -e "tail -f log/mds.cfg.log" 		-t "mds" 	--geometry=80x22+600+400
	gnome-terminal $(ARG) -e "tail -f log/osd.cfg.log" 		-t "osd" 	--geometry=80x22+0+800
	gnome-terminal $(ARG) -e "tail -f log/osd.cfg.2.log" 	-t "osd2" 	--geometry=80x22+600+800

# close log windows
logc:
	killall tail

#killall
k: 
	-killall -q cmgr.out osd.out mds.out mount.out

#run , restart all
r:
	./cmgr/cmgr.out									&
	./mds/mds.out									& 
	#./mds/mds.out -c etc/mcs.cfg.2					&
	./osd/osd.out									& 
	./osd/osd.out -c etc/osd.cfg.2					&



