CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow
SHARED = ../shared/Parse.c
SOURCE = Main.c Ports.c Threads.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE)) Parse.o
EXE    = CryptMsgServer

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) -pthread -o $@ $^

$(OBJECT): $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -c $^

gdb: $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -g $^
	$@ -args a.out 8080
	rm a.out

clean:
	rm $(OBJECT)
	rm $(EXE)
