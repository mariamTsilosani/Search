CFLAGS = -Wall -g

all: search index

index: index.o main.o
	gcc $(CFLAGS) index.o main.o -o index
index.o: index.c
	gcc $(CFLAGS) -c index.c -o index.o
main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o
search: search.o
	gcc $(CFLAGS) search.o -o search
search.o: search.c
	gcc $(CFLAGS) -c search.c -o search.o
clean: 
	rm -f search
	rm -f index
	rm -f *.o
       
