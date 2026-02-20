CC 	:= gcc
CFLAGS  := -lraylib -lsqlite3 -Iinclude -Wall -Wextra -Werror -g

BIN_DIR := bin
SRC_DIR := src

TARGET  := $(BIN_DIR)/main
SRC 	:= $(shell find $(SRC_DIR) -name *.c)

all: $(BIN_DIR)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
	$(TARGET)
