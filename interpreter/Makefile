CC=g++
CFLAGS=-Wall -O3
OUT_PATH=.

all: clean interpreter

interpreter: cJSON.o game.o interpreter.o
	$(CC) $(CFLAGS) interpreter.o cJSON.o game.o -o $(OUT_PATH)/interpreter.exe

interpreter.o:
	$(CC) $(CFLAGS) -c interpreter.cpp
	
cJSON.o:
	$(CC) $(CFLAGS) -c ../cJSON/cJSON.c

game.o:
	$(CC) $(CFLAGS) -c ../game.cpp
	
clean:
	del *.o