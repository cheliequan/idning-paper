all:
	./event_rpcgen.py test.rpc
	gcc -o server server.c test.gen.c -I/usr/local/include  -L/usr/local/lib -L/usr/lib -levent -g
	gcc -o client fuse.c client.c test.gen.c -I/usr/local/include  -L/usr/local/lib -L/usr/lib -levent  `pkg-config fuse --cflags --libs` -g -Wall 

c: clean
clean:
	rm -rf a.out client server
