CC=g++
CFLAGS=-I.
OBJ = main.o dash.o

%.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS)

dash: $(OBJ)
	$(CC) -o $@ $^

manage:
	g++ -pthread pidManager.cpp -o manage

clean :
	-rm $(OBJ)
