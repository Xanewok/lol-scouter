CC=g++
CFLAGS=-Wall -O3
LIBS=-lcurl
CLFLAGS=-L.\curl-7.34.0-devel-mingw32\bin
OUT_PATH=./bin

all: clean scouter

scouter: cJSON.o game.o scouter.o
	$(CC) $(CFLAGS) scouter.o cJSON.o game.o -o $(OUT_PATH)/scouter.exe $(LIBS) $(CLFLAGS)

scouter.o:
	$(CC) $(CFLAGS) -c scouter.cpp
	
cJSON.o:
	$(CC) $(CFLAGS) -c cJSON/cJSON.c

game.o:
	$(CC) $(CFLAGS) -c game.cpp
	
clean:
	del *.o