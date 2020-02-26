all: main test-me

main: main.o
	gcc -g -o main main.o

main.o: main.c
	gcc -g -c main.c

test-me:
	./main in.txt out.txt

clean:
	rm -f main main.o
