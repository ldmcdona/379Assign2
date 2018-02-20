make:
	gcc -std=c99 server.c -o server -lpthread
	gcc -std=c99 client.c -o client -lpthread
run:
	./server &
clean:
	rm server
	rm client
