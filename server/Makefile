CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow
SOURCE = Main.c Ports.c Message.c Runners.c Verify.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE))
LINKER = -L../shared/ -lshared
EXE    = CryptMsgServer

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) -pthread -o $@ $^ $(LINKER)

$(OBJECT): $(SOURCE)
	$(CC) $(FLAGS) -c $^

gdb: $(SOURCE)
	$(CC) $(FLAGS) -pthread -g $^ $(LINKER)
	$@ -args a.out -l -r
	rm a.out

clean:
	rm $(OBJECT)
	rm $(EXE)
