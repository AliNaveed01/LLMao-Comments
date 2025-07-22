CC = gcc
CFLAGS = -I./cJSON -I./include -I./src -Wall -Wextra -g
LDFLAGS = -lcurl

SRC = src/main.c src/utils.c src/parser.c src/api.c cJSON/cJSON.c
OBJ = $(SRC:.c=.o)
TARGET = build/code_commentor

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) src/*.o
