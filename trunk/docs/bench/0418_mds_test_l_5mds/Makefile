ARG=--window-with-profile=tmp --working-directory=/home/ning/idning-paper/src --hide-menubar
dist:
	scp -r /home/ning/idning-paper/src root@mds1:/tmp/
	scp -r /home/ning/idning-paper/src root@mds2:/tmp/
	scp -r /home/ning/idning-paper/src root@mds3:/tmp/
	scp -r /home/ning/idning-paper/src root@mds4:/tmp/
	ssh root@mds1 "cd /tmp/src/ && make " 
	ssh root@mds4 "cd /tmp/src/ && make " 
	ssh root@mds4 "scp -r /tmp/src root@mds2:/tmp/" 
	ssh root@mds4 "scp -r /tmp/src root@mds3:/tmp/" 

r:
	ssh root@cmgr "cd /tmp/src/ && ./cmgr/cmgr.out" &
	#
	
	ssh root@mds1 "cd /tmp/src && ./mds/mds.out -c etc/mds.cfg.1" &
	ssh root@mds2 "cd /tmp/src && ./mds/mds.out -c etc/mds.cfg.2" &
	ssh root@mds3 "cd /tmp/src && ./mds/mds.out -c etc/mds.cfg.3" &
	ssh root@mds4 "cd /tmp/src && ./mds/mds.out -c etc/mds.cfg.4" &
	ssh root@mds5 "cd /tmp/src && ./mds/mds.out -c etc/mds.cfg.5" &
	ssh root@mds6 "cd /tmp/src && ./mds/mds.out -c etc/mds.cfg.6" &
	ssh root@mds7 "cd /tmp/src && ./mds/mds.out -c etc/mds.cfg.7" &
	ssh root@mds8 "cd /tmp/src && ./mds/mds.out -c etc/mds.cfg.8" &
	#
	ssh root@osd1 "cd /tmp/src && ./osd/osd.out -c etc/osd.cfg" &
	ssh root@osd2 "cd /tmp/src && ./osd/osd.out -c etc/osd.cfg.2" &

k:
	#没用. $2有问题
	-ps aux | grep 'ssh root' | awk '{print $2}' | xargs kill -9
	#
	ssh root@mds1 "killall -q cmgr.out osd.out mds.out mount.out" &
	ssh root@mds2 "killall -q cmgr.out osd.out mds.out mount.out" &
	ssh root@mds3 "killall -q cmgr.out osd.out mds.out mount.out" &
	ssh root@mds4 "killall -q cmgr.out osd.out mds.out mount.out" &
	ssh root@mds5 "killall -q cmgr.out osd.out mds.out mount.out" &
	ssh root@mds6 "killall -q cmgr.out osd.out mds.out mount.out" &
	ssh root@mds7 "killall -q cmgr.out osd.out mds.out mount.out" &
	ssh root@mds8 "killall -q cmgr.out osd.out mds.out mount.out" &

ARG=--window-with-profile=tmp --working-directory=/home/ning/idning-paper/src --hide-menubar
t:
	cd /tmp/1/ && mkdir -p 1 2 3 4 5 6 7 8 a b c d e f g h
	#
	ssh localhost "cd /tmp/1/1 && fileop_ning -l1 -u22 -s0 > /tmp/fileop.1" &
	ssh localhost "cd /tmp/2/2 && fileop_ning -l1 -u22 -s0 > /tmp/fileop.2" &
	ssh localhost "cd /tmp/3/3 && fileop_ning -l1 -u22 -s0 > /tmp/fileop.3" &
	ssh localhost "cd /tmp/4/4 && fileop_ning -l1 -u22 -s0 > /tmp/fileop.4" &
	ssh localhost "cd /tmp/5/5 && fileop_ning -l1 -u22 -s0 > /tmp/fileop.5" &
	ssh localhost "cd /tmp/6/6 && fileop_ning -l1 -u22 -s0 > /tmp/fileop.6" &
	ssh localhost "cd /tmp/7/7 && fileop_ning -l1 -u22 -s0 > /tmp/fileop.7" &
	ssh localhost "cd /tmp/8/8 && fileop_ning -l1 -u22 -s0 > /tmp/fileop.8" &
	#
	#ssh localhost "cd /tmp/a/a && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.a" &
	#ssh localhost "cd /tmp/b/b && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.b" &
	#ssh localhost "cd /tmp/c/c && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.c" &
	#ssh localhost "cd /tmp/d/d && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.d" &
	#ssh localhost "cd /tmp/e/e && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.e" &
	#ssh localhost "cd /tmp/f/f && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.f" &
	#ssh localhost "cd /tmp/g/g && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.g" &
	#ssh localhost "cd /tmp/h/h && ~/idning-paper/iozone3_373/fileop -l1 -u22 -s0 > /tmp/fileop.h" &

create:
	cd /tmp/ && mkdir -p 1 2 3 4 5 6 7 8

m: u
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/1" 	-t "c1" 	--geometry=80x5+0+0&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/2" 	-t "c2" 	--geometry=80x5+0+100&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/3" 	-t "c3" 	--geometry=80x5+0+200&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/4" 	-t "c4" 	--geometry=80x5+0+300&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/5" 	-t "c5" 	--geometry=80x5+0+400&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/6" 	-t "c6" 	--geometry=80x5+0+500&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/7" 	-t "c7" 	--geometry=80x5+0+600&
	gnome-terminal $(ARG) -e "./client/mount.out /tmp/8" 	-t "c8" 	--geometry=80x5+0+700&
	#
	#gnome-terminal $(ARG) -e "./client/mount.out /tmp/a" 	-t "ca" 	--geometry=80x5+300+0&
	#gnome-terminal $(ARG) -e "./client/mount.out /tmp/b" 	-t "cb" 	--geometry=80x5+300+100&
	#gnome-terminal $(ARG) -e "./client/mount.out /tmp/c" 	-t "cc" 	--geometry=80x5+300+200&
	#gnome-terminal $(ARG) -e "./client/mount.out /tmp/d" 	-t "cd" 	--geometry=80x5+300+300&
	#gnome-terminal $(ARG) -e "./client/mount.out /tmp/e" 	-t "ce" 	--geometry=80x5+300+400&
	#gnome-terminal $(ARG) -e "./client/mount.out /tmp/f" 	-t "cf" 	--geometry=80x5+300+500&
	#gnome-terminal $(ARG) -e "./client/mount.out /tmp/g" 	-t "cg" 	--geometry=80x5+300+600&
	#gnome-terminal $(ARG) -e "./client/mount.out /tmp/h" 	-t "ch" 	--geometry=80x5+300+700&

f:
	cp /tmp/fileop.* ./
	cp -r /home/ning/idning-paper/src/log/ ./


u:
	-sudo umount /tmp/* -l


	

