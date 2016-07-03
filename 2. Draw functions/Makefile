CC=g++
CFLAGS=-c
LDFLAGS=-L/usr/lib/x86_64-linux-gnu -lglut -lGL -lGLU -lGLEW
IFLAGS=-I/usr/include
EXECUTABLE=draw
OFLAGS=-o $(EXECUTABLE)

all: draw

draw: LoadShaders.o main.o
	$(CC) $(OFLAGS) main.o LoadShaders.o $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

LoadShaders.o: LoadShaders.cpp
	$(CC) $(IFLAGS) $(CFLAGS) LoadShaders.cpp

clean: 
	rm -rf *.o $(EXECUTABLE) Debug Release -R
