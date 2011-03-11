#!/bin/bash
ulimit -c unlimite
D="/tmp/_$RANDOM"
echo 
echo $D
echo 
mkdir $D
gnome-terminal --window-with-profile=tmp --working-directory=$D --geometry=140x24&

#client/mount.out $D -d
client/mount.out $D 

#cd $D && test.sh

alias x='scripts/mount.sh'
#client/mount.out $D -d &
#pid=$!
#gdb -p $pid


#konsole --profile run

#umount /tmp/* -l
