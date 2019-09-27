CC = g++
FLUGS = -std=c++17

all:
	$(CC) $(FLUGS) -o exe main.cpp

clear:
	rm exe

