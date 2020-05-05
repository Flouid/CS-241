all: main test-me

main: main.o list.o calculator.o
	gcc -g -o main main.o list.o calculator.o

main.o: main.c
	gcc -g -c main.c

list.o: list.c
	gcc -g -c list.c

calculator.o: calculator.c
	gcc -g -c calculator.c

test-me:
	./main in.txt out.txt

clean:
	rm -f main main.o list.o calculator.o
