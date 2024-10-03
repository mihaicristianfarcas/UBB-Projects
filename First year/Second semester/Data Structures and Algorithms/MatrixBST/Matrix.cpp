#include <iostream>
#include "Matrix.h"

// Constructor
Matrix::Matrix(int nrLines, int nrCols) : root(nullptr), nr_lines(nrLines), nr_columns(nrCols) {}

// Destructor
Matrix::~Matrix() {
    destroyTree(root);
}

void Matrix::destroyTree(BSTNode* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

int Matrix::nrLines() const {
    return nr_lines;
}

int Matrix::nrColumns() const {
    return nr_columns;
}

Matrix::BSTNode* Matrix::insert(BSTNode* node, int line, int column, TElem value, TElem& old) {
    if (node == nullptr) {
        return new BSTNode(line, column, value);
    }

    if (line < node->line || (line == node->line && column < node->column)) {
        node->left = insert(node->left, line, column, value, old);
    } else if (line > node->line || (line == node->line && column > node->column)) {
        node->right = insert(node->right, line, column, value, old);
    } else {
        old = node->value;
        node->value = value;
    }

    return node;
}

Matrix::BSTNode* Matrix::find(BSTNode* node, int line, int column) const {
    if (node == nullptr || (node->line == line && node->column == column)) {
        return node;
    }

    if (line < node->line || (line == node->line && column < node->column)) {
        return find(node->left, line, column);
    } else {
        return find(node->right, line, column);
    }
}

Matrix::BSTNode* Matrix::minimum(BSTNode* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

Matrix::BSTNode* Matrix::remove(BSTNode* node, int line, int column, TElem& oldValue) {
    if (node == nullptr) {
        return node;
    }

    if (line < node->line || (line == node->line && column < node->column)) {
        node->left = remove(node->left, line, column, oldValue);
    } else if (line > node->line || (line == node->line && column > node->column)) {
        node->right = remove(node->right, line, column, oldValue);
    } else {
        oldValue = node->value;

        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }

        BSTNode* temp = minimum(node->right);
        node->line = temp->line;
        node->column = temp->column;
        node->value = temp->value;
        node->right = remove(node->right, temp->line, temp->column, oldValue);
    }

    return node;
}

TElem Matrix::element(int i, int j) const {
    if (i < 0 || i >= nr_lines || j < 0 || j >= nr_columns) {
        throw std::out_of_range("Invalid position");
    }

    BSTNode* node = find(root, i, j);
    return (node == nullptr) ? NULL_TELEM : node->value;
}

TElem Matrix::modify(int i, int j, TElem e) {
    if (i < 0 || i >= nr_lines || j < 0 || j >= nr_columns) {
        throw std::out_of_range("Invalid position");
    }

    TElem oldValue = NULL_TELEM;

    if (e != NULL_TELEM) {
        root = insert(root, i, j, e, oldValue);
    } else {
        root = remove(root, i, j, oldValue);
    }

    return oldValue;
}

void Matrix::setMainDiagonal(TElem elem) {
    if(nr_lines != nr_columns) {
        throw std::out_of_range("Not a square matrix");
    }

    for(int i = 0; i < nr_lines; i++) {
        modify(i, i, elem);
    }
}
