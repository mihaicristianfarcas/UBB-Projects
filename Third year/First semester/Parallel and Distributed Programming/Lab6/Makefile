CXX = g++
CXXFLAGS = -std=c++17 -pthread -O2 -Wall -Wextra

TARGET = hamiltonian_cpp
SOURCES = main.cpp
HEADERS = Graph.h HamiltonianCycleFinder.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
