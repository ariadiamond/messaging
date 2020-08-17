CC     = gcc
FLAGS  = -Wall -Wextra -Wpedantic -Wshadow
SHARED = ../shared/Parse.c
SOURCE = Main.c Connect.c Message.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE)) Parse.o
EXE    = Client

all: $(EXE)

$(EXE): $(OBJECT)
	$(CC) -o $@ $^

$(OBJECT): $(SOURCE) $(SHARED)
	$(CC) $(FLAGS) -c $^

clean:
	rm $(OBJECT) $(EXE)
