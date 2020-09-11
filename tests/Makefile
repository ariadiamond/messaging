CC    = gcc
FLAGS = -Wall -Wextra -Wpedantic -Wshadow

# Parsing
PARSE = ParseTest.c ../shared/Parse.c Test.c
POBJ  = ParseTest.o Parse.o Test.o
PEXE  = ParseTest

# Pseudo Client - designed for automation
LOCAL  = PseudoClient.c
CLIENT = Ports.c Message.c MessageHelper.c
SHARED = Parse.c Crypt.c Convert.c
SOURCE = $(LOCAL) $(patsubst %, ../client/%, $(CLIENT))\
	$(patsubst %, ../shared/%, $(SHARED))
SOBJ   = $(patsubst %.c, %.o, $(LOCAL)) $(patsubst %.c, %.o, $(CLIENT))\
	$(patsubst %.c, %.o,  $(SHARED))
SEXE   = PseudoClient

parse: $(PARSE)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(PEXE) $(POBJ)
	./$(PEXE)
	rm $(PEXE) $(POBJ)

pseudo: $(SOURCE)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(SEXE) $(SOBJ)

clean:
	rm *.o