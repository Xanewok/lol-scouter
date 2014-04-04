CC=gcc
CFLAGS=-Wall -O3
LIBS=-lcurl
CLFLAGS=-L.\curl-7.34.0-devel-mingw32\bin
OUT_PATH=./bin

all: clean scouter

scouter: scouter.o
	$(CC) $(CFLAGS) scouter.o -o $(OUT_PATH)/scouter.exe $(LIBS) $(CLFLAGS)

scouter.o:
	$(CC) $(CFLAGS) -c scouter.c
	
clean:
	del /Q *.o