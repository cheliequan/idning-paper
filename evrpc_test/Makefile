all:
	event_rpcgen.py test.rpc
	gcc -o server server.c test.gen.c -I/usr/local/include  -L/usr/local/lib -L/usr/lib -levent
	gcc -o client client.c test.gen.c -I/usr/local/include  -L/usr/local/lib -L/usr/lib -levent

clean:
	rm -rf a.out client server
