CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow
SHARED = $(patsubst %, ../shared/%, Parse.c Convert.c Ceaser.c)
SOURCE = Main.c Ports.c Message.c MessageHelper.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE)) $(patsubst ../shared/%.c, %.o, $(SHARED))
EXE    = Client

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) -o $@ $^

$(OBJECT): $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -c $^

clean:
	rm $(OBJECT) $(EXE)

config: Config.c
	$(CC) $(FLAGS) $^
	./a.out
	rm a.out
