#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

mutex print_mutex;

// Matrix structure
struct Matrix {
  vector<vector<int>> data;
  int rows, cols;

  Matrix(int r, int c) : rows(r), cols(c) { data.resize(r, vector<int>(c, 0)); }

  void randomize() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        data[i][j] = rand() % 10;
      }
    }
  }
};

// Function to compute a single element of the resulting matrix
void computeElement(const Matrix &A, const Matrix &B, Matrix &C, int row,
                    int col, int threadId, bool debug = false) {
  if (debug) {
    lock_guard<mutex> lock(print_mutex);
    cout << "Thread " << threadId << " computing element (" << row << ", "
         << col << ")" << endl;
  }

  int sum = 0;
  for (int k = 0; k < A.cols; k++) {
    sum += A.data[row][k] * B.data[k][col];
  }
  C.data[row][col] = sum;
}

// Approach 1: Each thread computes consecutive elements, row by row
void threadFunctionRowByRow(const Matrix &A, const Matrix &B, Matrix &C,
                            int threadId, int numThreads, bool debug = false) {
  int totalElements = C.rows * C.cols;
  int elementsPerThread = totalElements / numThreads;
  int startElement = threadId * elementsPerThread;
  int endElement = (threadId == numThreads - 1)
                       ? totalElements
                       : (threadId + 1) * elementsPerThread;

  for (int elem = startElement; elem < endElement; elem++) {
    int row = elem / C.cols;
    int col = elem % C.cols;
    computeElement(A, B, C, row, col, threadId, debug);
  }
}

// Approach 2: Each thread computes consecutive elements, column by column
void threadFunctionColumnByColumn(const Matrix &A, const Matrix &B, Matrix &C,
                                  int threadId, int numThreads,
                                  bool debug = false) {
  int totalElements = C.rows * C.cols;
  int elementsPerThread = totalElements / numThreads;
  int startElement = threadId * elementsPerThread;
  int endElement = (threadId == numThreads - 1)
                       ? totalElements
                       : (threadId + 1) * elementsPerThread;

  for (int elem = startElement; elem < endElement; elem++) {
    int col = elem / C.rows;
    int row = elem % C.rows;
    computeElement(A, B, C, row, col, threadId, debug);
  }
}

// Approach 3: Each thread takes every k-th element, going row by row
void threadFunctionEveryKth(const Matrix &A, const Matrix &B, Matrix &C,
                            int threadId, int numThreads, bool debug = false) {
  int totalElements = C.rows * C.cols;

  for (int elem = threadId; elem < totalElements; elem += numThreads) {
    int row = elem / C.cols;
    int col = elem % C.cols;
    computeElement(A, B, C, row, col, threadId, debug);
  }
}

// Function to run matrix multiplication with a specific approach
void runMatrixMultiplication(const Matrix &A, const Matrix &B, Matrix &C,
                             int numThreads, int approach, bool debug = false) {
  vector<thread> threads;

  auto start = high_resolution_clock::now();

  for (int i = 0; i < numThreads; i++) {
    if (approach == 1) {
      threads.emplace_back(threadFunctionRowByRow, ref(A), ref(B), ref(C), i,
                           numThreads, debug);
    } else if (approach == 2) {
      threads.emplace_back(threadFunctionColumnByColumn, ref(A), ref(B), ref(C),
                           i, numThreads, debug);
    } else if (approach == 3) {
      threads.emplace_back(threadFunctionEveryKth, ref(A), ref(B), ref(C), i,
                           numThreads, debug);
    }
  }

  for (auto &t : threads) {
    t.join();
  }

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end - start);

  cout << "Approach " << approach << " with " << numThreads
       << " threads: " << duration.count() / 1000.0 << " ms" << endl;
}

// Function to verify correctness (compare with sequential computation)
bool verifyResult(const Matrix &A, const Matrix &B, const Matrix &C) {
  for (int i = 0; i < C.rows; i++) {
    for (int j = 0; j < C.cols; j++) {
      int sum = 0;
      for (int k = 0; k < A.cols; k++) {
        sum += A.data[i][k] * B.data[k][j];
      }
      if (sum != C.data[i][j]) {
        return false;
      }
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  int matrixSize = 100;
  int numThreads = 4;
  bool debug = false;

  // Parse command line arguments
  if (argc > 1) {
    matrixSize = atoi(argv[1]);
  }
  if (argc > 2) {
    numThreads = atoi(argv[2]);
  }
  if (argc > 3) {
    debug = (string(argv[3]) == "debug");
  }

  cout << "Matrix Multiplication - Size: " << matrixSize << "x" << matrixSize
       << ", Threads: " << numThreads << endl;
  cout << string(60, '=') << endl;

  // Initialize matrices
  Matrix A(matrixSize, matrixSize);
  Matrix B(matrixSize, matrixSize);

  A.randomize();
  B.randomize();

  // Test each approach
  for (int approach = 1; approach <= 3; approach++) {
    Matrix C(matrixSize, matrixSize);
    runMatrixMultiplication(A, B, C, numThreads, approach, debug);

    // Verify correctness (only for smaller matrices to avoid long computation)
    if (matrixSize <= 100 && !debug) {
      if (verifyResult(A, B, C)) {
        cout << "  ✓ Result verified correct" << endl;
      } else {
        cout << "  ✗ Result verification failed!" << endl;
      }
    }
  }

  cout << string(60, '=') << endl;

  return 0;
}
