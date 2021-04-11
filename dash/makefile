CC=g++
CFLAGS=-I.
OBJ = main.o dash.o

%.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS)

dash: $(OBJ)
	$(CC) -o $@ $^

clean :
	-rm $(OBJ)
