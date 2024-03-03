#include "Matrix.h"

Matrix::Matrix(int rows, int cols) : _rows(rows), _cols(cols), _data(rows * cols) {}

int Matrix::rows() const { return _rows; }
int Matrix::cols() const { return _cols; }

double& Matrix::operator()(int row, int col) { return _data[row * _cols + col]; }
const double& Matrix::operator()(int row, int col) const { return _data[row * _cols + col]; }

double* Matrix::data() { return _data.data(); }
const double* Matrix::data() const { return _data.data(); }

void Matrix::readFromFile(const std::string& file_name) {
    std::ifstream input_file(file_name);

    // Check if the file is open.
    if (!input_file.is_open()) {
        throw std::runtime_error("Unable to open file: " + file_name);
    }

    // Read the dimensions of the matrix saved in the file
    input_file >> _rows >> _cols;
    _data.resize(_rows * _cols);

    // Iterate through all rows and columns and save the values to a one-dimensional vector
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            input_file >> _data[row * _cols + col];
        }
    }

    input_file.close();
}

void Matrix::saveToFile(const std::string& file_name) const {
    std::ofstream output_file(file_name);

    // Check if the file is open.
    if (!output_file.is_open()) {
        throw std::runtime_error("Unable to open file: " + file_name);
    }

    // Save the dimensions of the matrix to a file
    output_file << _rows << " " << _cols << std::endl;

    // Iterate through a one-dimensional vector, find the value for a given row and column, and save it to a file
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            output_file << _data[row * _cols + col] << ' ';
        }
        output_file << std::endl;
    }
    
    output_file.close();
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    matrix.print(os);
    return os;
}

void Matrix::print(std::ostream& os) const {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            os << _data[i * _cols + j] << ' ';
        }
        os << std::endl;
    }
}
