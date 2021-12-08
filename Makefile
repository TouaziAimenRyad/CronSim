CC = gcc
EXEC = cassini


cassini.o : src/cassini.c 
	$(CC) src/cassini.c -o $(EXEC)


distclean : 
	rm cassini