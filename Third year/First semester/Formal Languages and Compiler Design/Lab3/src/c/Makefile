CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -O2 -fPIC
SRC = symbol_table.c
BUILD_DIR = ../../build

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    TARGET = $(BUILD_DIR)/libsymbol_table.dylib
    SHARED_FLAG = -dynamiclib
else
    TARGET = $(BUILD_DIR)/libsymbol_table.so
    SHARED_FLAG = -shared
endif

all: $(TARGET)

$(TARGET): $(SRC) symbol_table.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAG) -o $@ $(SRC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(BUILD_DIR)/libsymbol_table.dylib $(BUILD_DIR)/libsymbol_table.so

.PHONY: all clean
