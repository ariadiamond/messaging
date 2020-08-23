CC    = gcc
FLAGS = -Wall -Wextra -Wpedantic -Wshadow

# Parsing
PARSE = ParseTest.c ../shared/Parse.c Test.c
POBJ  = ParseTest.o Parse.o Test.o
PEXE  = ParseTest

# Pseudo Client - designed for automation
SUDO  = PseudoClient.c ../client/Ports.c ../client/Message*.c ../shared/Parse.c
SOBJ  = PseudoClient.o Ports.o Parse.o Message*.o
SEXE  = PseudoClient

parse: $(PARSE)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(PEXE) $(POBJ)
	./$(PEXE)
	rm $(PEXE) $(POBJ)

pseudo: $(SUDO)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(SEXE) $(SOBJ)
	rm $(SOBJ)
