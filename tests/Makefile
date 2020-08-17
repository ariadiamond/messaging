CC    = gcc
FLAGS = -Wall -Wextra -Wpedantic -Wshadow

# Parsing
PARSE = ParseTest.c ../shared/Parse.c Test.c
POBJ  = ParseTest.o Parse.o Test.o
PEXE  = ParseTest

# Queue
QUEUE = QueueTest.c ../shared/Queue.c Test.c
QOBJ  = QueueTest.o Queue.o Test.o
QEXE  = QueueTest

# Pseudo Client - designed for automation
SUDO  = PseudoClient.c ../client/Connect.c ../client/Message.c ../shared/Parse.c
SOBJ  = PseudoClient.o Connect.o Parse.o Message.o
SEXE  = PseudoClient

parse: $(PARSE)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(PEXE) $(POBJ)
	./$(PEXE)
	rm $(PEXE) $(POBJ)

queue: $(QUEUE)
	$(CC) $(FLAGS) -c $^
	$(CC) -pthread -o $(QEXE) $(QOBJ)
	./$(QEXE)
	rm $(QEXE) $(QOBJ)

pseudo: $(SUDO)
	$(CC) $(FLAGS) -c $^
	$(CC) -o $(SEXE) $(SOBJ)

clean:
	rm $(SOBJ) $(SEXE)
