#pragma once
#include <stdexcept>

typedef int TElem;
#define NULL_TELEM 0

class Matrix {

private:
    struct BSTNode {
        int line;
        int column;
        TElem value;
        BSTNode* left;
        BSTNode* right;

        BSTNode(int l, int c, TElem v) : line(l), column(c), value(v), left(nullptr), right(nullptr) {}
    };

    BSTNode* root;
    int nr_lines;
    int nr_columns;

    BSTNode* insert(BSTNode* node, int line, int column, TElem value, TElem& old);

    BSTNode* find(BSTNode* node, int line, int column) const;

    BSTNode* remove(BSTNode* node, int line, int column, TElem& oldValue);

    BSTNode* minimum(BSTNode* node) const;

    void destroyTree(BSTNode* node);

public:
    // Constructor
    Matrix(int nrLines, int nrCols);

    // Destructor
    ~Matrix();

    // Returns the number of lines
    [[nodiscard]] int nrLines() const;

    // Returns the number of columns
    [[nodiscard]] int nrColumns() const;

    // Returns the element from line i and column j (indexing starts from 0)
    // Throws exception if (i,j) is not a valid position in the Matrix
    [[nodiscard]] TElem element(int i, int j) const;

    // Modifies the value from line i and column j
    // Returns the previous value from the position
    // Throws exception if (i,j) is not a valid position in the Matrix
    TElem modify(int i, int j, TElem e);

    void setMainDiagonal(TElem elem);
};
