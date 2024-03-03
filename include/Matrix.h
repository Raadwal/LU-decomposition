#pragma once

#include <iostream>
#include <fstream>
#include <vector>

// A class representing a matrix of double values
class Matrix {
public:
    // Constructs a matrix with the given number of rows and columns
    Matrix(int rows, int cols);

    // Returns the number of rows in the matrix
    int rows() const;
    // Returns the number of columns in the matrix
    int cols() const;

    // Returns a pointer to the raw data of the matrix
    double* data();
    // Returns a const pointer to the raw data of the matrix
    const double* data() const;

    // Returns the reference to the element at the given row and column
    double& operator()(int row, int col);
    // Returns the const reference to the element at the given row and column
    const double& operator()(int row, int col) const;

    // Reads the matrix data from a file with the given file name
    void readFromFile(const std::string& file_name);
    // Reads the matrix data from a file with the given file name
    void saveToFile(const std::string& file_name) const;

    // Overloads the output stream operator for printing the matrix
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

private:
    // Number of rows
    int _rows;
    // Number of columns
    int _cols;
    // One dimensional vector storing matrix's data
    std::vector<double> _data;

    // Overloads the output stream operator for printing the matrix.
    void print(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const Matrix& matrix);