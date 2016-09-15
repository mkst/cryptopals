CC = g++
CFLAGS = -std=c++11 -Wall

LIBS = -lcryptopp
INCLUDES = -I./include

SOURCES = src/main.cpp $(wildcard src/helpers/*.cpp) $(wildcard src/challenges/*.cpp)
OUTPUT = ./bin


all:
	mkdir -p $(OUTPUT)
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS) $(INCLUDES) -o $(OUTPUT)/a.out

clean:
	rm $(OUTPUT)/*.out
