CC=gcc
CFLAGS=-I.
DEPS = main.h cmdpu.h currencylsd.h gameobjects.h inventory.h list.h location.h player.h
OBJ = main.o cmdpu.o currencylsd.o gameobjects.o inventory.o list.o location.o player.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

aRPG: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

