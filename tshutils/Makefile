CC=g++
CFLAGS=-c -Wall -Wextra -std=c++11
LDFLAGS=-std=c++11 -L../libpathm/out -lpathm -lboost_system
SOURCES=src/Shell.cpp src/main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tsh

OBJF=out/obj

.PHONY: createfolders
.PHONY: all

all: | createfolders list new newdir rmall size 
	

createfolders: 
	-mkdir -p out/obj/


list: list.o
	$(CC) $(OBJF)/$? -o out/$@ $(LDFLAGS)

new: new.o
	$(CC) $(OBJF)/$? -o out/$@ $(LDFLAGS)

newdir: newdir.o
	$(CC) $(OBJF)/$? -o out/$@ $(LDFLAGS)

rmall: rmall.o
	$(CC) $(OBJF)/$? -o out/$@ $(LDFLAGS)

size: size.o
	$(CC) $(OBJF)/$? -o out/$@ $(LDFLAGS)


list.o: src/list.cpp
	$(CC) $(CFLAGS) $? -o $(OBJF)/$@

new.o: src/new.cpp
	$(CC) $(CFLAGS) $? -o $(OBJF)/$@

newdir.o: src/newdir.cpp
	$(CC) $(CFLAGS) $? -o $(OBJF)/$@

rmall.o: src/rmall.cpp
	$(CC) $(CFLAGS) $? -o $(OBJF)/$@

size.o: src/size.cpp
	$(CC) $(CFLAGS) $? -o $(OBJF)/$@

clean :
	rm -f *.o
	rm -f tsh
	rm -f *~

