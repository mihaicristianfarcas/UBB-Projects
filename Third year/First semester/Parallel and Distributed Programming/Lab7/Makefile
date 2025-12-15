.PHONY: all clean test run

CXX = mpic++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Wpedantic
SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/polymul_mpi

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(SOURCES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -rf $(BIN_DIR)

# Run with default parameters (4 processes)
run: $(TARGET)
	mpirun -n 4 $(TARGET)

# Run comprehensive tests with different process counts
test: $(TARGET)
	@echo "=== Running comprehensive tests ==="
	@echo ""
	@echo "Test 1: Small degree (degree=100, 2 processes)"
	mpirun -n 2 $(TARGET) --degree 100 --repetitions 3
	@echo ""
	@echo "Test 2: Medium degree (degree=500, 4 processes)"
	mpirun -n 4 $(TARGET) --degree 500 --repetitions 3
	@echo ""
	@echo "Test 3: Large degree (degree=1000, 8 processes)"
	mpirun -n 8 $(TARGET) --degree 1000 --repetitions 3
	@echo ""
	@echo "=== All tests completed ==="

# Scaling test: run with 1, 2, 4, 8 processes
scaling: $(TARGET)
	@echo "=== Scaling Test (degree=1000) ==="
	@for n in 1 2 4 8; do \
		echo ""; \
		echo "--- $$n process(es) ---"; \
		mpirun -n $$n $(TARGET) --degree 1000 --repetitions 5; \
	done
