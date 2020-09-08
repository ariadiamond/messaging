CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow
SHARED = ../shared/Parse.c ../shared/Convert.c ../shared/Crypt.c
SOURCE = Main.c Ports.c Message.c MessageHelper.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE)) $(patsubst ../shared/%.c, %.o, $(SHARED))
EXE    = Client

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) -o $@ $^

$(OBJECT): $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -c $^

gdb: $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -g $^
	$@ --args $(EXE) aria x
	rm a.out

clean:
	rm $(OBJECT) $(EXE)

config: Config.c
	$(CC) $(FLAGS) $^
	./a.out
	rm a.out