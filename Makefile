CC=g++
CFLAGS=-Wall -O3
LIBS=-lcurl
CLFLAGS=-L.\curl-7.34.0-devel-mingw32\bin
OUT_PATH=./bin

all: clean scouter interpreter

scouter: cJSON.o game.o scouter.o static_data.o network.o
	$(CC) $(CFLAGS) scouter.o cJSON.o game.o static_data.o network.o -o $(OUT_PATH)/scouter.exe $(LIBS) $(CLFLAGS)

scouter.o:
	$(CC) $(CFLAGS) -c scouter.cpp

interpreter: cJSON.o team_game.o game.o interpreter.o static_data.o
	$(CC) $(CFLAGS) interpreter.o cJSON.o team_game.o game.o static_data.o -o $(OUT_PATH)/interpreter.exe

interpreter.o:
	$(CC) $(CFLAGS) -c interpreter.cpp
	
cJSON.o:
	$(CC) $(CFLAGS) -c cJSON/cJSON.c

game.o:
	$(CC) $(CFLAGS) -c game.cpp
	
team_game.o:
	$(CC) $(CFLAGS) -c team_game.cpp
	
static_data.o:
	$(CC) $(CFLAGS) -c static_data.cpp
	
network.o:
	$(CC) $(CFLAGS) -c network.cpp
	
clean:
	del *.o