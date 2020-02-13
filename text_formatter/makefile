CC = gcc
SRC = formatter
EXEC = formatter
TEXT = shakespeare

all: $(EXEC) test-me

$(EXEC): $(SRC).o list.o
	$(CC) -g -o $(EXEC) $(SRC).o list.o

$(SRC).o: $(SRC).c
	$(CC) -g -c $(SRC).c

list.o: list.c
	$(CC) -g -c list.c

test-me:
	./$(EXEC) $(TEXT).txt out.txt 50

clean:
	rm -f $(EXEC) $(SRC).o list.o
