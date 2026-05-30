CC=gcc
CFLAGS=-Wall -Wextra -Werror
SOURCES=src/threadpool.c src/main.c
OBJECTS=$(patsubst src/%.c, build/%.o, $(SOURCES))
TARGET=bin/threadpool
INCLUDES=inc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -I$(INCLUDES) $(OBJECTS) -o $(TARGET)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

clean:
	rm -f build/*
	rm -f $(TARGET)
