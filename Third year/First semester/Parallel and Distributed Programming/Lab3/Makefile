CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread -Wall
TARGET = matrix_multiply

all: $(TARGET)

$(TARGET): matrix_multiply.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) matrix_multiply.cpp

clean:
	rm -f $(TARGET)

# Run with default parameters (100x100 matrix, 4 threads)
run: $(TARGET)
	./$(TARGET)

# Run with debug output (small 9x9 matrix)
debug: $(TARGET)
	./$(TARGET) 9 4 debug

# Performance tests with various matrix sizes
test-sizes: $(TARGET)
	@echo "Testing different matrix sizes..."
	@echo ""
	@echo "Size: 50x50"
	./$(TARGET) 50 4
	@echo ""
	@echo "Size: 100x100"
	./$(TARGET) 100 4
	@echo ""
	@echo "Size: 200x200"
	./$(TARGET) 200 4
	@echo ""
	@echo "Size: 500x500"
	./$(TARGET) 500 4

# Performance tests with various thread counts
test-threads: $(TARGET)
	@echo "Testing different thread counts (200x200 matrix)..."
	@echo ""
	@echo "Threads: 1"
	./$(TARGET) 200 1
	@echo ""
	@echo "Threads: 2"
	./$(TARGET) 200 2
	@echo ""
	@echo "Threads: 4"
	./$(TARGET) 200 4
	@echo ""
	@echo "Threads: 8"
	./$(TARGET) 200 8
	@echo ""
	@echo "Threads: 16"
	./$(TARGET) 200 16

.PHONY: all clean run debug test-sizes test-threads
