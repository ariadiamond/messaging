CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow
SHARED = ../shared/Parse.c
SOURCE = Main.c Ports.c Message.c Runners.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE)) Parse.o
EXE    = CryptMsgServer

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) -pthread -o $@ $^

$(OBJECT): $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -c $^

gdb: $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -pthread -g $^
	$@ -args a.out -l -r
	rm a.out

clean:
	rm $(OBJECT)
	rm $(EXE)
