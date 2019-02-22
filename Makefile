all: build

build:
	gcc -Wall main.c HashTables.c hash.c -o hash_table

run:
	./hash_table

clean:
	rm -fr *.o hash_table
