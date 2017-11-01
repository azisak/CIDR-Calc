make:
	gcc -o client src/client.c
run:
	./client
clean:
	rm -f client
