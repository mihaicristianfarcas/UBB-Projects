CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -Ic_parser -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lcjson -lm

SRC_DIR = c_parser
BUILD_DIR = build
BIN_DIR = bin

SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/grammar.c \
          $(SRC_DIR)/set.c \
          $(SRC_DIR)/parser.c \
          $(SRC_DIR)/json_loader.c

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET = $(BIN_DIR)/ll1_parser

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

test: $(TARGET)
	@echo "Running parser test..."
	$(TARGET) --grammar config/grammar.json --pif samples/program1_pif.txt --tokens config/tokens.json --output validation/c_output.txt

.PHONY: install-deps
install-deps:
	@echo "Installing dependencies with Homebrew..."
	@brew list cjson >/dev/null 2>&1 || brew install cjson
