SOURCE = Convert.c Crypt.c Parse.c
OBJECT = $(patsubst %c, %o, $(SOURCE))
LIB    = libshared.a

all: $(LIB)

$(LIB): $(OBJECT)
	ar -cvq $@ $^
	rm $(OBJECT)

$(OBJECT): $(SOURCE)
	gcc -Wall -Wextra -Wpedantic -Wshadow -c $^