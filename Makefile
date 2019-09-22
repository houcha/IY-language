CC=g++
CFLAGS=-std=c++17
COMMON_SOURCES=cmp.cpp output.cpp sort.cpp text.cpp

all: main tests

main: $(COMMON_SOURCES) main.cpp
	$(CC) $(CFLAGS) $(COMMON_SOURCES) main.cpp -o $@

tests: $(COMMON_SOURCES) tests.cpp
	$(CC) $(CFLAGS) $(COMMON_SOURCES) tests.cpp -o $@

tags:
	ctags -R . > tags

clean:
	rm main tests tags
