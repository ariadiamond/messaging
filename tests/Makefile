CC    = gcc
FLAGS = -Wall -Wextra -Wpedantic -Wshadow

# Parsing
PARSE = ParseTest.c ../shared/Parse.c Test.c
POBJ  = ParseTest.o Parse.o Test.o
PEXE  = ParseTest

# Pseudo Client - designed for automation
LOCAL  = PseudoClient.c
CLIENT = ../client/Ports.c ../client/Message*.c
SHARED = ../shared/Parse.c ../shared/Crypt.c ../shared/Convert.c
SOURCE = $(LOCAL) $(CLIENT) $(SHARED)
SOBJ   = $(patsubst %.c, %.o, $(LOCAL)) $(patsubst ../client/%.c, %.o, $(CLIENT)) $(patsubst ../shared/%.c, %.o,  $(SHARED))
SEXE   = PseudoClient

parse: $(PARSE)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(PEXE) $(POBJ)
	./$(PEXE)
	rm $(PEXE) $(POBJ)

pseudo: $(SOURCE)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(SEXE) $(SOBJ)
