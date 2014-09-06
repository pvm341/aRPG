CC=gcc
LD=g++
CFLAGS=-c -Wall -std=c99 -O
LDFLAGS=
SOURCES=ansiutils.c cmdpu.c csvdata.c currencylsd.c gameobjects.c inventory.c list.c location.c main.c mob.c  player.c
WORLDBDRS=ansiutils.c cmdpu.c csvdata.c currencylsd.c gameobjects.c inventory.c list.c location.c mob.c player.c worldbuilder.c

#main.cpp hello.cpp factorial.cpp
OBJECTS=$(SOURCES:.c=.o)
WORLDBDRO=$(WORLDBDRS:.c=.o)

EXECUTABLE=aRPG
WRLDBDR=worldbuilder

arpg:$(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
 
wb: $(WORLDBDRS) $(WRLDBDR)

$(WRLDBDR): $(WORLDBDRO)
	$(CC) $(LDFLAGS) $(WORLDBDRO) -o $@

clean:
	-rm -f *.o aRPG worldbuilder
 
all: 
	$(EXECUTABLE) $(WORLDBDRS)
	 
  
.c.o:
	$(CC) $(CFLAGS) $< -o $@

