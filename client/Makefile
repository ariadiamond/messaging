CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow
SOURCE = Main.c Ports.c Message.c MessageHelper.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE))
LINKER = -L../shared/ -lshared
EXE    = Client

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) -o $@ $^ $(LINKER)

$(OBJECT): $(SOURCE)
	$(CC) $(FLAGS) -c $^

gdb: $(SOURCE)
	$(CC) $(FLAGS) -g $^ $(LINKER)
	$@ --args $(EXE) aria x
	rm a.out

clean:
	rm $(OBJECT) $(EXE)

config: Config.c
	$(CC) $(FLAGS) $^
	./a.out
	rm a.out
