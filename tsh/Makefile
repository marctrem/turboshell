CC=g++
CFLAGS=-c -Wall -Wextra -std=c++11
LDFLAGS=-std=c++11 -L../libpathm/out -lpathm -lboost_system
SOURCES=src/Shell.cpp src/main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tsh

OBJF=out/obj

tsh: | createfolders main.o Shell.o
	$(CC) $(OBJF)/main.o $(OBJF)/Shell.o -o out/tsh $(LDFLAGS)
.PHONY: createfolders

createfolders: 
	-mkdir -p out/obj/

main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o $(OBJF)/$@

Shell.o: src/Shell.cpp
	$(CC) $(CFLAGS) src/Shell.cpp -o $(OBJF)/$@


clean :
	rm -f *.o
	rm -f tsh
	rm -f *~

