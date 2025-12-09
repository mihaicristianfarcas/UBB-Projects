CC = gcc
CFLAGS = -Wall -Wextra -O3 -g -I/opt/homebrew/include  # O3 for max optimization
LDFLAGS = -L/opt/homebrew/lib -lcjson -lm

# Directories
V0 = optimizations/v0_baseline
V1 = optimizations/v1_set
V2 = optimizations/v2_grammar
V3 = optimizations/v3_combined

# Build directories
BUILD_V0 = build/v0
BUILD_V1 = build/v1
BUILD_V2 = build/v2
BUILD_V3 = build/v3

# Source files
SOURCES = grammar.c set.c parser.c json_loader.c main.c

# Binaries
BIN_V0 = bin/parser_v0
BIN_V1 = bin/parser_v1
BIN_V2 = bin/parser_v2
BIN_V3 = bin/parser_v3

.PHONY: all clean benchmark compare

all: $(BIN_V0) $(BIN_V1) $(BIN_V2) $(BIN_V3)

# V0 - Baseline
$(BUILD_V0):
	mkdir -p $(BUILD_V0)

$(BUILD_V0)/%.o: $(V0)/%.c | $(BUILD_V0)
	$(CC) $(CFLAGS) -I$(V0) -c $< -o $@

$(BIN_V0): $(addprefix $(BUILD_V0)/, $(SOURCES:.c=.o)) | bin
	$(CC) $^ -o $@ $(LDFLAGS)

# V1 - Set optimizations
$(BUILD_V1):
	mkdir -p $(BUILD_V1)

$(BUILD_V1)/%.o: $(V1)/%.c | $(BUILD_V1)
	$(CC) $(CFLAGS) -I$(V1) -c $< -o $@

$(BIN_V1): $(addprefix $(BUILD_V1)/, $(SOURCES:.c=.o)) | bin
	$(CC) $^ -o $@ $(LDFLAGS)

# V2 - Grammar indexing
$(BUILD_V2):
	mkdir -p $(BUILD_V2)

$(BUILD_V2)/%.o: $(V2)/%.c | $(BUILD_V2)
	$(CC) $(CFLAGS) -I$(V2) -c $< -o $@

$(BIN_V2): $(addprefix $(BUILD_V2)/, $(SOURCES:.c=.o)) | bin
	$(CC) $^ -o $@ $(LDFLAGS)

# V3 - Combined
$(BUILD_V3):
	mkdir -p $(BUILD_V3)

$(BUILD_V3)/%.o: $(V3)/%.c | $(BUILD_V3)
	$(CC) $(CFLAGS) -I$(V3) -c $< -o $@

$(BIN_V3): $(addprefix $(BUILD_V3)/, $(SOURCES:.c=.o)) | bin
	$(CC) $^ -o $@ $(LDFLAGS)

bin:
	mkdir -p bin

clean:
	rm -rf build bin results/*.txt

# Benchmark all versions
benchmark: all
	@echo "=== Running Benchmarks ==="
	@echo
	@echo "V0 - Baseline:"
	@/usr/bin/time -l $(BIN_V0) --grammar config/grammar.json --pif samples/program1_pif.txt --tokens config/tokens.json --output results/v0_output.txt 2>&1 | grep -E "real|maximum resident"
	@echo
	@echo "V1 - Set Optimizations:"
	@/usr/bin/time -l $(BIN_V1) --grammar config/grammar.json --pif samples/program1_pif.txt --tokens config/tokens.json --output results/v1_output.txt 2>&1 | grep -E "real|maximum resident"
	@echo
	@echo "V2 - Grammar Indexing:"
	@/usr/bin/time -l $(BIN_V2) --grammar config/grammar.json --pif samples/program1_pif.txt --tokens config/tokens.json --output results/v2_output.txt 2>&1 | grep -E "real|maximum resident"
	@echo
	@echo "V3 - Combined:"
	@/usr/bin/time -l $(BIN_V3) --grammar config/grammar.json --pif samples/program1_pif.txt --tokens config/tokens.json --output results/v3_output.txt 2>&1 | grep -E "real|maximum resident"

# Compare outputs
compare: benchmark
	@echo
	@echo "=== Comparing Outputs ==="
	@diff results/v0_output.txt results/v1_output.txt && echo "✓ V0 == V1" || echo "✗ V0 != V1"
	@diff results/v0_output.txt results/v2_output.txt && echo "✓ V0 == V2" || echo "✗ V0 != V2"
	@diff results/v0_output.txt results/v3_output.txt && echo "✓ V0 == V3" || echo "✗ V0 != V3"
