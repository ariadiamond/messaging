CC    = gcc
FLAGS = -Wall -Wextra -Wpedantic -Wshadow

# Parsing
PARSE = ParseTest.c ../shared/Parse.c Test.c
POBJ  = ParseTest.o Parse.o Test.o
PEXE  = ParseTest

# Pseudo Client - designed for automation
LOCAL  = PseudoClient.c
CLIENT = Ports.c Message.c MessageHelper.c
SOURCE = $(LOCAL) $(patsubst %, ../client/%, $(CLIENT))
SOBJ   = $(patsubst %.c, %.o, $(LOCAL)) $(patsubst %.c, %.o, $(CLIENT))
LINKER = -L../shared/ -lshared
SEXE   = PseudoClient

parse: $(PARSE)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(PEXE) $(POBJ)
	./$(PEXE)
	rm $(PEXE) $(POBJ)

pseudo: $(SOURCE)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(SEXE) $(SOBJ) $(LINKER)

clean:
	rm *.o
