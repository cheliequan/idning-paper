all:
	echo "" > cscope.files
	find /home/ning/idning-source/c/libevent-2.0.10-stable/  -name "*.h" >> cscope.files
	find /home/ning/idning-source/c/libevent-2.0.10-stable/  -name "*.c" >> cscope.files
	find . -name "*.c" >> cscope.files
	cscope  -i cscope.files -R -b
	gcc -o client libevent_client.c -I/usr/local/include  -L/usr/local/lib -levent  -g

c: clean
clean:
	rm -rf a.out client server
