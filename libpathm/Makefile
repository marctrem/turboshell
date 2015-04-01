CC=g++
CFLAGS=-c -Wall -Wextra -std=c++11
LDFLAGS=-lboost_system -std=c++11
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tsh

all: libpathm

.PHONY: createfolders

createfolders: 
	-mkdir -p out/obj/

libpathm: | createfolders path.o file_util.o
	ar rcs out/libpathm.a out/obj/path.o out/obj/file_util.o

path.o: src/path.cpp
	$(CC) $(CFLAGS) $? -o out/obj/$@

file_util.o: src/file_util.cpp
	$(CC) $(CFLAGS) $? -o out/obj/$@

clean :
	rm -f *.o
	rm -f tsh
	rm -f *~