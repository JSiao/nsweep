#This will be fixed soon

CC=gcc
CFLAGS= -std=c11
LFLAGS= -lncurses
EXE=nsweep

all:$(EXE)

$(EXE): $(EXE).c
	$(CC) $(EXE).c -o$(EXE) $(CFLAGS) $(LFLAGS)

clean:
	rm -rf $(EXE)
