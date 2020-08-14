CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow -pthread
SHARED = ../shared/Parse.c
SOURCE = Main.c Ports.c Threads.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE)) Parse.o
EXE    = CryptMessageServer

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) $(FLAGS) -o $@ $^

$(OBJECT): $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -c $^

clean:
	rm $(OBJECT)
	rm $(EXE)
