CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

# Source files
SRC_ROOT = ../src/eic.c
SRC_BF = ../src/interpreters/brainfuck/brainfuck.c
SRC_BEFUNGE = $(wildcard ../src/interpreters/funge/*.c)
SRC_WS = $(wildcard ../src/interpreters/whitespace/*.c)
SRC_RUNTIME = $(wildcard ../src/runtime/*.c)

# Combine all sources
SRCS = $(SRC_ROOT) $(SRC_BF) $(SRC_BEFUNGE) $(SRC_WS) $(SRC_RUNTIME)

# Object files go next to sources, with .o extension
OBJS = $(SRCS:.c=.o)

# Final executable
TARGET = eic

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
