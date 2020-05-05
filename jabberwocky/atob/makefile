CC = gcc
SRC = atob
EXEC = atob
INTEXT = wright
OUTTEXT = bit-wright

all: $(EXEC) test-me

$(EXEC): $(SRC).o
	$(CC) -g -o $(EXEC) $(SRC).o

$(SRC).o: $(SRC).c
	$(CC) -g -c $(SRC).c

test-me:
	./$(EXEC) $(INTEXT).txt $(OUTTEXT).txt

clean:
	rm -f $(EXEC) $(SRC).o
