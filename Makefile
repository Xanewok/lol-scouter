CC=g++
CFLAGS=-Wall -O3
LIBS=-lcurl
CLFLAGS=-L.\curl-7.34.0-devel-mingw32\bin
OUT_PATH=./bin

all: clean scouter interpreter

scouter: scouter.o cJSON.o team_game.o game.o static_data.o io.o network.o
	$(CC) $(CFLAGS) scouter.o cJSON.o team_game.o game.o static_data.o io.o network.o -o $(OUT_PATH)/scouter.exe $(LIBS) $(CLFLAGS)

interpreter: cJSON.o team_game.o game.o interpreter.o static_data.o io.o network.o
	$(CC) $(CFLAGS) interpreter.o cJSON.o team_game.o game.o static_data.o io.o network.o -o $(OUT_PATH)/interpreter.exe $(LIBS) $(CLFLAGS)
	
scouter.o:
	$(CC) $(CFLAGS) -c scouter.cpp

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

io.o:
	$(CC) $(CFLAGS) -c io.cpp
	
clean:
	del *.o