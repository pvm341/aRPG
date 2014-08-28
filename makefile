CC=gcc
LD=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=$(LS) *.c
#main.cpp hello.cpp factorial.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=aRPG

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

