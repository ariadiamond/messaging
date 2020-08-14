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
