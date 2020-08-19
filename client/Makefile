CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow
SHARED = ../shared/Parse.c
SOURCE = Main.c Ports.c Message.c MessageHelper.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE)) Parse.o
EXE    = Client

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) -o $@ $^

$(OBJECT): $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -c $^

clean:
	rm $(OBJECT) $(EXE)
