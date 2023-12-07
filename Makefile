CC = g++
CFLAGS = -Wall -pthread

all: mapper

mapper: main.cpp HashTable.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f mapper
