CC=gcc
CFLAGS=-I. -std=c99
DEPS = main.h cmdpu.h currencylsd.h gameobjects.h inventory.h list.h location.h player.h ansiutils.h csvdata.h
OBJ = main.o cmdpu.o currencylsd.o gameobjects.o inventory.o list.o location.o player.o ansiutils.o csvdata.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

aRPG: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

