CC=g++
CFLAGS=-c
LDFLAGS=-L/usr/lib/x86_64-linux-gnu -lglut -lGL -lGLU -lGLEW
IFLAGS=-I/usr/include
EXECUTABLE=triangles
OFLAGS=-o $(EXECUTABLE)

all: triangles

triangles: LoadShaders.o triangles.o
	$(CC) $(OFLAGS) triangles.o LoadShaders.o $(LDFLAGS)

triangles.o: triangles.cpp
	$(CC) $(CFLAGS) triangles.cpp

LoadShaders.o: LoadShaders.cpp
	$(CC) $(IFLAGS) $(CFLAGS) LoadShaders.cpp

clean: 
	rm -rf *.o $(EXECUTABLE) Debug Release -R
