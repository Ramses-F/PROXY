CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/proxy.c src/logger.c src/cache.c
OBJ = $(SRC:.c=.o)
TARGET = proxy

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
