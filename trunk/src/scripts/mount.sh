#!/bin/bash
D="/tmp/$RANDOM"
echo 
echo $D
echo 
mkdir $D
gnome-terminal --window-with-profile=tmp --working-directory=$D &

client/mount.out $D -d

cd $D && test.sh

#client/mount.out $D -d &
#pid=$!
#gdb -p $pid


#konsole --profile run
